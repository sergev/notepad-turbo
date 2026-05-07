#include "nn_test_helpers.h"
#include "NNDocument.h"

// Direct unit tests for NNDocument's Scintilla::IDocument interface.
// Pattern mirrors Scintilla's testDocument.cxx (DocPlus wrapper).
//
// TEditor (Turbo Vision) converts bare '\n' to '\r\n' on insert, so
// "hello\nworld\n" becomes "hello\r\nworld\r\n" in the flat buffer.
// Line positions in these tests are computed accordingly.

// --------------------------------------------------------------------------
// Buffer access: Version, Length, GetCharRange, BufferPointer
// --------------------------------------------------------------------------

TEST(NNDocument, VersionIsRelease4)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->Version(), Scintilla::dvRelease4);
}

TEST(NNDocument, LengthEmptyBuffer)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->Length(), 0);
}

TEST(NNDocument, LengthAfterInsertNoNewline)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->Length(), 5);
}

TEST(NNDocument, LengthCountsCRLFExpansion)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    // '\n' is expanded to '\r\n', so 3 lines × 2 bytes + 3 content bytes = 9
    ctx.insertText("a\nb\n");
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->Length(), 6);  // "a\r\nb\r\n" = 6 bytes
}

TEST(NNDocument, GetCharRangeFull)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    char buf[6] = {};
    doc->GetCharRange(buf, 0, 5);
    EXPECT_EQ(std::string(buf, 5), "hello");
}

TEST(NNDocument, GetCharRangePartial)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    char buf[4] = {};
    doc->GetCharRange(buf, 1, 3);
    EXPECT_EQ(std::string(buf, 3), "ell");
}

TEST(NNDocument, GetCharRangeClipsAtBufferEnd)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hi");
    NNDocument *doc = ctx.editor->nnDocument();

    char buf[10] = {};
    doc->GetCharRange(buf, 0, 100);  // overshoots — must not crash
    EXPECT_EQ(std::string(buf, 2), "hi");
}

TEST(NNDocument, BufferPointerContainsText)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("abc");
    NNDocument *doc = ctx.editor->nnDocument();

    const char *ptr = doc->BufferPointer();
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(std::string(ptr, 3), "abc");
}

// --------------------------------------------------------------------------
// Style pipeline: StartStyling, SetStyleFor, SetStyles, StyleAt
// --------------------------------------------------------------------------

TEST(NNDocument, StyleAtEmptyBuffer)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->StyleAt(0), '\0');
}

TEST(NNDocument, StyleAtOutOfRange)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hi");
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->StyleAt(-1), '\0');
    EXPECT_EQ(doc->StyleAt(100), '\0');
}

TEST(NNDocument, SetStyleForFillsRange)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->StartStyling(0);
    doc->SetStyleFor(5, (char)3);

    EXPECT_EQ((unsigned char)doc->StyleAt(0), 3u);
    EXPECT_EQ((unsigned char)doc->StyleAt(4), 3u);
    EXPECT_EQ(doc->StyleAt(5), '\0');  // one past styled range
}

TEST(NNDocument, SetStyleForAdvancesPosition)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->StartStyling(0);
    doc->SetStyleFor(2, (char)1);  // "he" → style 1
    doc->SetStyleFor(3, (char)2);  // "llo" → style 2

    EXPECT_EQ((unsigned char)doc->StyleAt(0), 1u);
    EXPECT_EQ((unsigned char)doc->StyleAt(1), 1u);
    EXPECT_EQ((unsigned char)doc->StyleAt(2), 2u);
    EXPECT_EQ((unsigned char)doc->StyleAt(4), 2u);
}

TEST(NNDocument, SetStylesPerByteArray)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("abc");
    NNDocument *doc = ctx.editor->nnDocument();

    const char styles[] = {(char)1, (char)2, (char)3};
    doc->StartStyling(0);
    doc->SetStyles(3, styles);

    EXPECT_EQ((unsigned char)doc->StyleAt(0), 1u);
    EXPECT_EQ((unsigned char)doc->StyleAt(1), 2u);
    EXPECT_EQ((unsigned char)doc->StyleAt(2), 3u);
}

TEST(NNDocument, StartStylingResetsCursor)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("abcde");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->StartStyling(0);
    doc->SetStyleFor(5, (char)9);

    // Re-style the middle portion only
    doc->StartStyling(2);
    doc->SetStyleFor(2, (char)7);  // positions 2–3

    EXPECT_EQ((unsigned char)doc->StyleAt(0), 9u);
    EXPECT_EQ((unsigned char)doc->StyleAt(1), 9u);
    EXPECT_EQ((unsigned char)doc->StyleAt(2), 7u);
    EXPECT_EQ((unsigned char)doc->StyleAt(3), 7u);
    EXPECT_EQ((unsigned char)doc->StyleAt(4), 9u);
}

// --------------------------------------------------------------------------
// Line navigation: LineFromPosition, LineStart, LineEnd
//
// After insertText("hello\nworld\n") TEditor produces "hello\r\nworld\r\n":
//   h(0) e(1) l(2) l(3) o(4) \r(5) \n(6) w(7) o(8) r(9) l(10) d(11) \r(12) \n(13)
// --------------------------------------------------------------------------

TEST(NNDocument, LineFromPositionSingleLine)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    EXPECT_EQ(doc->LineFromPosition(0), 0);
    EXPECT_EQ(doc->LineFromPosition(4), 0);
}

TEST(NNDocument, LineFromPositionMultiLine)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("hello\nworld\n");
    NNDocument *doc = ctx.editor->nnDocument();

    EXPECT_EQ(doc->LineFromPosition(0),  0);  // 'h'
    EXPECT_EQ(doc->LineFromPosition(4),  0);  // 'o'
    EXPECT_EQ(doc->LineFromPosition(5),  0);  // '\r'
    EXPECT_EQ(doc->LineFromPosition(7),  1);  // 'w' — after '\n' at pos 6
    EXPECT_EQ(doc->LineFromPosition(11), 1);  // 'd'
}

TEST(NNDocument, LineStartMultiLine)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("hello\nworld\n");
    NNDocument *doc = ctx.editor->nnDocument();

    EXPECT_EQ(doc->LineStart(0), 0);
    EXPECT_EQ(doc->LineStart(1), 7);  // 'w' follows '\n' at pos 6
}

TEST(NNDocument, LineStartPastLastLineReturnsLength)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    EXPECT_EQ(doc->LineStart(1), doc->Length());
}

TEST(NNDocument, LineEndMultiLine)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("hello\nworld\n");
    NNDocument *doc = ctx.editor->nnDocument();

    EXPECT_EQ(doc->LineEnd(0), 5);   // '\r' at pos 5
    EXPECT_EQ(doc->LineEnd(1), 12);  // '\r' at pos 12
}

// --------------------------------------------------------------------------
// Fold levels: SetLevel, GetLevel
// --------------------------------------------------------------------------

TEST(NNDocument, FoldLevelDefaultZero)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetLevel(0),  0);
    EXPECT_EQ(doc->GetLevel(99), 0);
}

TEST(NNDocument, FoldLevelRoundTrip)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("a\nb\nc\n");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->SetLevel(0, 0x400);
    doc->SetLevel(1, 0x401);
    doc->SetLevel(2, 0x400);

    EXPECT_EQ(doc->GetLevel(0), 0x400);
    EXPECT_EQ(doc->GetLevel(1), 0x401);
    EXPECT_EQ(doc->GetLevel(2), 0x400);
}

TEST(NNDocument, FoldLevelOutOfRangeReturnsZero)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("a\n");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->SetLevel(0, 5);
    EXPECT_EQ(doc->GetLevel(0),  5);
    EXPECT_EQ(doc->GetLevel(99), 0);  // never set
}

// --------------------------------------------------------------------------
// Lexer state: SetLineState, GetLineState
// --------------------------------------------------------------------------

TEST(NNDocument, LineStateDefaultZero)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetLineState(0),  0);
    EXPECT_EQ(doc->GetLineState(99), 0);
}

TEST(NNDocument, LineStateRoundTrip)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("a\nb\n");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->SetLineState(0, 42);
    doc->SetLineState(1, 7);

    EXPECT_EQ(doc->GetLineState(0), 42);
    EXPECT_EQ(doc->GetLineState(1), 7);
}

TEST(NNDocument, LineStateOutOfRangeReturnsZero)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("a\n");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->SetLineState(0, 99);
    EXPECT_EQ(doc->GetLineState(0),  99);
    EXPECT_EQ(doc->GetLineState(50), 0);
}

// --------------------------------------------------------------------------
// Indentation: GetLineIndentation
// --------------------------------------------------------------------------

TEST(NNDocument, IndentationNone)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("abc\n");
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetLineIndentation(0), 0);
}

TEST(NNDocument, IndentationSpaces)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("   abc\n");
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetLineIndentation(0), 3);
}

TEST(NNDocument, IndentationTabs)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("\t\tabc\n");
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetLineIndentation(0), 8);  // 2 tabs × 4
}

TEST(NNDocument, IndentationMixedSpaceTab)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText(" \tabc\n");  // 1 space + 1 tab
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetLineIndentation(0), 5);  // 1 + 4
}

// --------------------------------------------------------------------------
// Encoding: CodePage, IsDBCSLeadByte
// --------------------------------------------------------------------------

TEST(NNDocument, CodePageIsUTF8)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->CodePage(), 65001);
}

TEST(NNDocument, IsDBCSLeadByteAlwaysFalse)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_FALSE(doc->IsDBCSLeadByte('a'));
    EXPECT_FALSE(doc->IsDBCSLeadByte('\x80'));
    EXPECT_FALSE(doc->IsDBCSLeadByte('\xff'));
}

// --------------------------------------------------------------------------
// Character navigation: GetRelativePosition, GetCharacterAndWidth
// --------------------------------------------------------------------------

TEST(NNDocument, GetRelativePosition)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();
    EXPECT_EQ(doc->GetRelativePosition(0, 0), 0);
    EXPECT_EQ(doc->GetRelativePosition(3, 2), 5);
    EXPECT_EQ(doc->GetRelativePosition(10, 0), 10);
}

TEST(NNDocument, GetCharacterAndWidth)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hi");
    NNDocument *doc = ctx.editor->nnDocument();

    Sci_Position width = 0;
    EXPECT_EQ(doc->GetCharacterAndWidth(0, &width), 'h');
    EXPECT_EQ(width, 1);
    EXPECT_EQ(doc->GetCharacterAndWidth(1, &width), 'i');
    EXPECT_EQ(width, 1);
}

TEST(NNDocument, GetCharacterAndWidthOutOfRange)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    NNDocument *doc = ctx.editor->nnDocument();

    Sci_Position width = 99;
    EXPECT_EQ(doc->GetCharacterAndWidth(0, &width), 0);   // empty buffer
    EXPECT_EQ(doc->GetCharacterAndWidth(-1, &width), 0);  // negative position
}

// --------------------------------------------------------------------------
// ChangeLexerState — smoke test (internal effect not observable directly)
// --------------------------------------------------------------------------

TEST(NNDocument, ChangeLexerStateDoesNotCrash)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    NNDocument *doc = ctx.editor->nnDocument();

    doc->ChangeLexerState(0, 5);
    doc->ChangeLexerState(2, 3);  // backward region — min should be kept
}
