#include "util.h"
#include "MacroRecorder.h"

TEST(MacroRecorderTest, StartAndStop)
{
    MacroRecorder rec;
    rec.startRecording();
    Macro *m = rec.stopRecording();
    ASSERT_NE(m, nullptr);
    delete m;
}

TEST(MacroRecorderTest, StopWithoutStartReturnsNull)
{
    MacroRecorder rec;
    Macro *m = rec.stopRecording();
    EXPECT_EQ(m, nullptr);
}

TEST(MacroRecorderTest, RecordWhenNotRecordingIsNoOp)
{
    MacroRecorder rec;
    EXPECT_NO_THROW(rec.recordStep(NNMacroCmd::Delete));
}

TEST(MacroRecorderTest, RecordSingleStep)
{
    MacroRecorder rec;
    rec.startRecording();
    rec.recordStep(NNMacroCmd::CharLeft);
    Macro *m = rec.stopRecording();
    ASSERT_NE(m, nullptr);
    EXPECT_EQ(m->size(), 1);
    EXPECT_EQ(m->getSteps()[0].cmd, NNMacroCmd::CharLeft);
    delete m;
}

TEST(MacroRecorderTest, RecordMultipleSteps)
{
    MacroRecorder rec;
    rec.startRecording();
    rec.recordStep(NNMacroCmd::CharLeft);
    rec.recordStep(NNMacroCmd::Delete);
    rec.recordStep(NNMacroCmd::LineEnd);
    Macro *m = rec.stopRecording();
    ASSERT_NE(m, nullptr);
    ASSERT_EQ(m->size(), 3);
    EXPECT_EQ(m->getSteps()[0].cmd, NNMacroCmd::CharLeft);
    EXPECT_EQ(m->getSteps()[1].cmd, NNMacroCmd::Delete);
    EXPECT_EQ(m->getSteps()[2].cmd, NNMacroCmd::LineEnd);
    delete m;
}

TEST(MacroRecorderTest, InsertTextMergedThroughRecorder)
{
    MacroRecorder rec;
    rec.startRecording();
    rec.recordStep(NNMacroCmd::InsertText, "hello");
    rec.recordStep(NNMacroCmd::InsertText, " world");
    Macro *m = rec.stopRecording();
    ASSERT_NE(m, nullptr);
    ASSERT_EQ(m->size(), 1);
    EXPECT_EQ(m->getSteps()[0].text, "hello world");
    delete m;
}
