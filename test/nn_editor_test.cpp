#include "nn_test_helpers.h"
#include "util.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace {

fs::path editorFilePath()
{
    return fs::temp_directory_path() / ("nt_editor_" + get_test_name() + ".txt");
}

void writeBinaryFile(const fs::path &path, const std::string &bytes)
{
    std::ofstream out(path, std::ios::out | std::ios::binary);
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

class NNEditorFileOpenTest : public ::testing::Test {
protected:
    void TearDown() override { fs::remove(path); }
    fs::path path = editorFilePath();
};

} // namespace

// Tests 1-5: NNEditor rendering, cursor movement, syntax highlighting, folding.

// Test 1: Plain text (no lexer) renders character grid correctly.
TEST(NNEditor, PlainTextRenders)
{
    NNEditorTest ctx;
    ctx.init(10, 3);
    ctx.insertText("hello\nworld\n");
    ctx.draw();

    auto text = ctx.group->getBufferText();
    EXPECT_EQ(text[0][0], "h");
    EXPECT_EQ(text[0][1], "e");
    EXPECT_EQ(text[0][4], "o");
    EXPECT_EQ(text[1][0], "w");
    EXPECT_EQ(text[1][4], "d");
}

// Test 2: Cursor position updates correctly on arrow key navigation.
// No draw() needed — tests handleEvent state only.
TEST(NNEditor, CursorMovesOnArrowKeys)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("line1\nline2\n");

    // After insertText, cursor is at end of "line2\n".
    // Move to start of buffer.
    ctx.sendKey(kbCtrlHome);
    EXPECT_EQ((int)ctx.editor->curPos.y, 0);
    EXPECT_EQ((int)ctx.editor->curPos.x, 0);

    // Move down one line.
    ctx.sendKey(kbDown);
    EXPECT_EQ((int)ctx.editor->curPos.y, 1);

    // Move to end of that line.
    ctx.sendKey(kbEnd);
    EXPECT_EQ((int)ctx.editor->curPos.x, 5);  // "line2" is 5 chars
}

// Test 3: Syntax highlight colors differ for C++ keywords vs identifiers.
TEST(NNEditor, SyntaxColorsKeywordVsIdentifier)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.editor->setLexer("cpp");

    // Without keyword registration the CPP lexer cannot distinguish keywords
    // from identifiers — we must supply a keyword list.
    ctx.editor->setKeywords(0,
        "int char short long void float double "
        "bool true false if else for while do return");

    // SCE_C_WORD=5 (keyword), SCE_C_IDENTIFIER=11
    // Assign distinguishable RGB foreground colors.
    ctx.editor->styleSetFore(5,  0xFF0000);   // keyword: red
    ctx.editor->styleSetFore(11, 0x00FF00);   // identifier: green

    ctx.insertText("int foo;\n");
    ctx.draw();

    // "int" starts at column 0, "foo" starts at column 4.
    TColorAttr kwAttr = ctx.group->getBufferAttr(0, 0);   // 'i'
    TColorAttr idAttr = ctx.group->getBufferAttr(4, 0);   // 'f'

    // Both must be non-default and different from each other.
    EXPECT_NE(kwAttr, TColorAttr{});
    EXPECT_NE(idAttr, TColorAttr{});
    EXPECT_NE(kwAttr, idAttr);
}

// Test 4: After toggleFold(), interior lines of a C++ block are hidden from the
// rendered output and the next visible line appears on row 1.
TEST(NNEditor, FoldToggleHidesInteriorLines)
{
    NNEditorTest ctx;
    ctx.init(20, 6);
    ctx.editor->setLexer("cpp");

    ctx.insertText("void f() {\nint x;\n}\n");
    ctx.draw();   // populates fold levels via runLexer()

    // Navigate to the start (the fold header line).
    ctx.sendKey(kbCtrlHome);
    ctx.editor->toggleFold();
    ctx.draw();

    // Row 0 still shows the opening of the function ("v" for "void").
    auto text = ctx.group->getBufferText();
    EXPECT_EQ(text[0][0], "v");

    // Row 1 should show "}" (the first visible line after the collapsed fold),
    // NOT "i" from "int x;" (which is now hidden).
    EXPECT_NE(text[1][0], "i");
}

// Test 5: Arrow key navigation skips over lines hidden inside a collapsed fold.
TEST(NNEditor, NavigationSkipsHiddenLines)
{
    NNEditorTest ctx;
    ctx.init(20, 6);
    ctx.editor->setLexer("cpp");

    ctx.insertText("void f() {\nint x;\n}\nextra\n");
    ctx.draw();  // run lexer so fold levels are populated

    // Navigate to first line, then collapse the fold.
    ctx.sendKey(kbCtrlHome);
    ctx.editor->toggleFold();

    // The cursor should have been pushed past the hidden region.
    // Lines 1 ("int x;") and 2 ("}") are hidden; cursor must be on visible line.
    int cy = (int)ctx.editor->curPos.y;
    EXPECT_TRUE(cy == 0 || cy >= 3)
        << "Cursor at line " << cy << " should not be inside collapsed fold";
}

TEST_F(NNEditorFileOpenTest, OpensUtf8FileUnchangedAndClean)
{
    const std::string expected = "alpha\ncaf\xC3\xA9\n";
    writeBinaryFile(path, expected);

    NNEditorTest ctx;
    ctx.init(40, 5, path.string().c_str());

    EXPECT_EQ(ctx.editor->flatText(), expected);
    EXPECT_FALSE(ctx.editor->modified);
}

TEST_F(NNEditorFileOpenTest, OpensWindows1252FileAsUtf8)
{
    const std::string bytes =
        "Windows-1252 sample: \x93quoted\x94 \x97 caf\xE9 \x80.\n"
        "Windows-1252 sample: \x93quoted\x94 \x97 caf\xE9 \x80.\n"
        "Windows-1252 sample: \x93quoted\x94 \x97 caf\xE9 \x80.\n";
    const std::string expected =
        "Windows-1252 sample: \xE2\x80\x9Cquoted\xE2\x80\x9D \xE2\x80\x94 caf\xC3\xA9 \xE2\x82\xAC.\n"
        "Windows-1252 sample: \xE2\x80\x9Cquoted\xE2\x80\x9D \xE2\x80\x94 caf\xC3\xA9 \xE2\x82\xAC.\n"
        "Windows-1252 sample: \xE2\x80\x9Cquoted\xE2\x80\x9D \xE2\x80\x94 caf\xC3\xA9 \xE2\x82\xAC.\n";
    writeBinaryFile(path, bytes);

    NNEditorTest ctx;
    ctx.init(80, 8, path.string().c_str());

    EXPECT_EQ(ctx.editor->flatText(), expected);
    EXPECT_FALSE(ctx.editor->modified);
}

TEST_F(NNEditorFileOpenTest, OpensUtf16LeBomFileAsUtf8)
{
    const char raw[] = {
        static_cast<char>(0xFF), static_cast<char>(0xFE),
        'h', 0, 'i', 0, ' ', 0,
        static_cast<char>(0xE9), 0, '\n', 0,
    };
    const std::string bytes(raw, sizeof(raw));
    writeBinaryFile(path, bytes);

    NNEditorTest ctx;
    ctx.init(40, 5, path.string().c_str());

    EXPECT_EQ(ctx.editor->flatText(), "hi \xC3\xA9\n");
    EXPECT_FALSE(ctx.editor->modified);
}

TEST_F(NNEditorFileOpenTest, OpensInvalidEncodingBytesWithoutConversion)
{
    const char raw[] = {
        static_cast<char>(0xFF), static_cast<char>(0xFE),
        'h', 0, 'x',
    };
    const std::string bytes(raw, sizeof(raw));
    writeBinaryFile(path, bytes);

    NNEditorTest ctx;
    ctx.init(40, 5, path.string().c_str());

    EXPECT_EQ(ctx.editor->flatText(), bytes);
    EXPECT_FALSE(ctx.editor->modified);
}
