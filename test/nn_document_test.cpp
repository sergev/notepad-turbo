#include "nn_test_helpers.h"

// Tests 7-8: NNDocument IDocument interface (accessed via NNEditor).

// Test 7: textLength() matches the number of bytes inserted.
TEST(NNDocument, LengthMatchesInserted)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("hello");
    EXPECT_EQ(ctx.editor->textLength(), 5);
}

// Test 8: flatText() round-trips gap-buffer content faithfully.
// TEditor converts bare '\n' to '\r\n' on insert (Borland TUI convention).
TEST(NNDocument, FlatTextRoundTrips)
{
    NNEditorTest ctx;
    ctx.init(20, 5);
    ctx.insertText("a\nb\nc\n");
    EXPECT_EQ(ctx.editor->flatText(), "a\r\nb\r\nc\r\n");
}

// Test 9: textLength() is 0 for a fresh editor.
TEST(NNDocument, EmptyEditorLength)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    EXPECT_EQ(ctx.editor->textLength(), 0);
    EXPECT_EQ(ctx.editor->flatText(), "");
}

// Test 10: cursorPos() reflects cursor after insertText.
TEST(NNDocument, CursorPosAfterInsert)
{
    NNEditorTest ctx;
    ctx.init(20, 3);
    ctx.insertText("abc");
    // After inserting 3 chars the cursor is at position 3.
    EXPECT_EQ(ctx.editor->cursorPos(), 3);
}
