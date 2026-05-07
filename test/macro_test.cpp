#include "util.h"
#include "Macro.h"
#include "MacroManager.h"
#include "IniSettings.h"

#include <filesystem>

// ---- Macro storage tests ----

TEST(MacroTest, DefaultName)
{
    Macro m;
    EXPECT_EQ(m.getName(), "<Current Recorded Macro>");
}

TEST(MacroTest, SetGetName)
{
    Macro m;
    m.setName("MyMacro");
    EXPECT_EQ(m.getName(), "MyMacro");
}

TEST(MacroTest, InitiallyEmpty)
{
    Macro m;
    EXPECT_EQ(m.size(), 0);
    EXPECT_TRUE(m.getSteps().empty());
}

TEST(MacroTest, AddSingleStep)
{
    Macro m;
    m.addStep(NNMacroCmd::Delete);
    ASSERT_EQ(m.size(), 1);
    EXPECT_EQ(m.getSteps()[0].cmd, NNMacroCmd::Delete);
    EXPECT_TRUE(m.getSteps()[0].text.empty());
}

TEST(MacroTest, ConsecutiveInsertTextMerges)
{
    Macro m;
    m.addStep(NNMacroCmd::InsertText, "ab");
    m.addStep(NNMacroCmd::InsertText, "cd");
    ASSERT_EQ(m.size(), 1);
    EXPECT_EQ(m.getSteps()[0].text, "abcd");

    m.addStep(NNMacroCmd::InsertText, "ef");
    ASSERT_EQ(m.size(), 1);
    EXPECT_EQ(m.getSteps()[0].text, "abcdef");
}

TEST(MacroTest, MergeStopsOnOtherCommand)
{
    Macro m;
    m.addStep(NNMacroCmd::InsertText, "a");
    m.addStep(NNMacroCmd::Delete);
    m.addStep(NNMacroCmd::InsertText, "b");
    ASSERT_EQ(m.size(), 3);
    EXPECT_EQ(m.getSteps()[0].text, "a");
    EXPECT_EQ(m.getSteps()[1].cmd, NNMacroCmd::Delete);
    EXPECT_EQ(m.getSteps()[2].text, "b");
}

TEST(MacroTest, AddStepByValue)
{
    Macro m;
    m.addStep(NNMacroStep(NNMacroCmd::CharLeft));
    ASSERT_EQ(m.size(), 1);
    EXPECT_EQ(m.getSteps()[0].cmd, NNMacroCmd::CharLeft);
}

// ---- MacroManager settings persistence tests ----

namespace fs = std::filesystem;

static fs::path macro_tmp_path()
{
    static int counter = 0;
    return fs::temp_directory_path() / ("nt_macro_mgr_" + std::to_string(++counter) + ".ini");
}

class MacroManagerSettingsTest : public ::testing::Test {
protected:
    void SetUp() override { path = macro_tmp_path(); }
    void TearDown() override { fs::remove(path); }
    fs::path path;
};

TEST_F(MacroManagerSettingsTest, SaveAndLoadEmptyMacroList)
{
    {
        IniSettings ini(path.string());
        MacroManager mgr;
        mgr.saveSettings(ini);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    MacroManager mgr2;
    mgr2.loadSettings(ini2);
    EXPECT_TRUE(mgr2.availableMacros().empty());
}

TEST_F(MacroManagerSettingsTest, SaveAndLoadSingleMacro)
{
    {
        IniSettings ini(path.string());
        MacroManager mgr;
        // Manually build a macro (bypass recording which needs NNEditor)
        Macro *m = new Macro();
        m->setName("TestMacro");
        m->addStep(NNMacroCmd::CharLeft);
        m->addStep(NNMacroCmd::Delete);
        mgr.availableMacros().push_back(m);
        mgr.saveSettings(ini);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    MacroManager mgr2;
    mgr2.loadSettings(ini2);
    ASSERT_EQ(mgr2.availableMacros().size(), 1u);
    const Macro *m = mgr2.availableMacros()[0];
    EXPECT_EQ(m->getName(), "TestMacro");
    ASSERT_EQ(m->size(), 2);
    EXPECT_EQ(m->getSteps()[0].cmd, NNMacroCmd::CharLeft);
    EXPECT_EQ(m->getSteps()[1].cmd, NNMacroCmd::Delete);
}

TEST_F(MacroManagerSettingsTest, SaveAndLoadInsertTextStep)
{
    {
        IniSettings ini(path.string());
        MacroManager mgr;
        Macro *m = new Macro();
        m->setName("TextMacro");
        m->addStep(NNMacroCmd::InsertText, "hello");
        mgr.availableMacros().push_back(m);
        mgr.saveSettings(ini);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    MacroManager mgr2;
    mgr2.loadSettings(ini2);
    ASSERT_EQ(mgr2.availableMacros().size(), 1u);
    const Macro *m = mgr2.availableMacros()[0];
    ASSERT_EQ(m->size(), 1);
    EXPECT_EQ(m->getSteps()[0].cmd, NNMacroCmd::InsertText);
    EXPECT_EQ(m->getSteps()[0].text, "hello");
}

TEST_F(MacroManagerSettingsTest, LoadSkipsMacrosWithEmptyName)
{
    IniSettings ini(path.string());
    ini.setInt("Macros.count", 1);
    ini.set("Macros.macro0.name", "");
    ini.setInt("Macros.macro0.steps", 0);
    MacroManager mgr;
    mgr.loadSettings(ini);
    EXPECT_TRUE(mgr.availableMacros().empty());
}

TEST_F(MacroManagerSettingsTest, LoadMultipleMacros)
{
    {
        IniSettings ini(path.string());
        MacroManager mgr;
        for (int i = 0; i < 3; ++i) {
            Macro *m = new Macro();
            m->setName("Macro" + std::to_string(i));
            mgr.availableMacros().push_back(m);
        }
        mgr.saveSettings(ini);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    MacroManager mgr2;
    mgr2.loadSettings(ini2);
    ASSERT_EQ(mgr2.availableMacros().size(), 3u);
    EXPECT_EQ(mgr2.availableMacros()[0]->getName(), "Macro0");
    EXPECT_EQ(mgr2.availableMacros()[2]->getName(), "Macro2");
}
