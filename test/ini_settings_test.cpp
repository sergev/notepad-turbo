#include "util.h"
#include "IniSettings.h"

#include <climits>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

static fs::path tmp_path()
{
    return fs::temp_directory_path() / ("nt_test_" + get_test_name() + ".ini");
}

class IniSettingsTest : public ::testing::Test {
protected:
    void TearDown() override { fs::remove(path); }
    fs::path path = tmp_path();
};

TEST_F(IniSettingsTest, GetMissingKeyReturnsDefault)
{
    IniSettings ini(path.string());
    EXPECT_EQ(ini.get("a.b"), "");
    EXPECT_EQ(ini.get("a.b", "def"), "def");
    EXPECT_EQ(ini.getInt("a.b", 42), 42);
    EXPECT_EQ(ini.getBool("a.b", true), true);
    EXPECT_EQ(ini.getBool("a.b", false), false);
}

TEST_F(IniSettingsTest, SetAndGetString)
{
    IniSettings ini(path.string());
    ini.set("sec.key", "hello");
    EXPECT_EQ(ini.get("sec.key"), "hello");
    EXPECT_EQ(ini.get("sec.other", "x"), "x");
}

TEST_F(IniSettingsTest, SetAndGetInt)
{
    IniSettings ini(path.string());
    ini.setInt("a.n", 123);
    EXPECT_EQ(ini.getInt("a.n"), 123);
    ini.set("a.bad", "notanumber");
    EXPECT_EQ(ini.getInt("a.bad", 99), 99);
}

TEST_F(IniSettingsTest, SetAndGetBool)
{
    IniSettings ini(path.string());
    ini.setBool("a.flag", true);
    EXPECT_TRUE(ini.getBool("a.flag"));
    ini.setBool("a.flag", false);
    EXPECT_FALSE(ini.getBool("a.flag"));

    // Alternative truthy values
    ini.set("a.x", "1");   EXPECT_TRUE(ini.getBool("a.x"));
    ini.set("a.y", "yes"); EXPECT_TRUE(ini.getBool("a.y"));
    ini.set("a.z", "0");   EXPECT_FALSE(ini.getBool("a.z"));
}

TEST_F(IniSettingsTest, RemoveKey)
{
    IniSettings ini(path.string());
    ini.set("sec.k", "v");
    ini.remove("sec.k");
    EXPECT_EQ(ini.get("sec.k", "gone"), "gone");
}

TEST_F(IniSettingsTest, RemoveSection)
{
    IniSettings ini(path.string());
    ini.set("sec.a", "1");
    ini.set("sec.b", "2");
    ini.set("other.x", "3");
    ini.removeSection("sec");
    EXPECT_EQ(ini.get("sec.a", "gone"), "gone");
    EXPECT_EQ(ini.get("sec.b", "gone"), "gone");
    EXPECT_EQ(ini.get("other.x"), "3");
}

TEST_F(IniSettingsTest, KeysWithPrefix)
{
    IniSettings ini(path.string());
    ini.set("sec.a", "1");
    ini.set("sec.b", "2");
    ini.set("other.c", "3");

    auto keys = ini.keysWithPrefix("sec.");
    ASSERT_EQ(keys.size(), 2u);
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "sec.a") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "sec.b") != keys.end());

    auto other = ini.keysWithPrefix("other.");
    ASSERT_EQ(other.size(), 1u);
    EXPECT_EQ(other[0], "other.c");

    EXPECT_TRUE(ini.keysWithPrefix("missing.").empty());
}

TEST_F(IniSettingsTest, SaveAndLoad)
{
    {
        IniSettings ini(path.string());
        ini.set("S1.key1", "val1");
        ini.setInt("S1.key2", 42);
        ini.setBool("S2.flag", true);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    EXPECT_EQ(ini2.get("S1.key1"), "val1");
    EXPECT_EQ(ini2.getInt("S1.key2"), 42);
    EXPECT_TRUE(ini2.getBool("S2.flag"));
}

TEST_F(IniSettingsTest, LoadNonexistentFile)
{
    IniSettings ini("/tmp/does_not_exist_xyz_abc.ini");
    EXPECT_NO_THROW(ini.load());
    EXPECT_EQ(ini.get("a.b", "def"), "def");
}

TEST_F(IniSettingsTest, LoadCRLFLineEndings)
{
    {
        std::ofstream f(path);
        f << "[Section]\r\nkey=value\r\n";
    }
    IniSettings ini(path.string());
    ini.load();
    EXPECT_EQ(ini.get("Section.key"), "value");
}

TEST_F(IniSettingsTest, OverwriteExistingKey)
{
    IniSettings ini(path.string());
    ini.set("a.k", "first");
    ini.set("a.k", "second");
    EXPECT_EQ(ini.get("a.k"), "second");
}

TEST_F(IniSettingsTest, SaveAndLoadLowerState)
{
    {
        IniSettings ini(path.string());
        ini.set("S1.str", "");
        ini.setInt("S1.num", 0);
        ini.setBool("S1.flag", false);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    EXPECT_EQ(ini2.get("S1.str"), "");
    EXPECT_EQ(ini2.getInt("S1.num"), 0);
    EXPECT_FALSE(ini2.getBool("S1.flag"));
    // Confirm stored zero/false are distinct from a truly absent key
    EXPECT_EQ(ini2.getInt("S1.missing", 99), 99);
    EXPECT_EQ(ini2.getBool("S1.missing", true), true);
}

TEST_F(IniSettingsTest, SaveAndLoadUpperState)
{
    {
        IniSettings ini(path.string());
        ini.set("S1.str", "Hello, World! Foo=Bar; Baz#Qux");
        ini.setInt("S1.num", INT_MAX);
        ini.setBool("S1.flag", true);
        ini.save();
    }
    IniSettings ini2(path.string());
    ini2.load();
    EXPECT_EQ(ini2.get("S1.str"), "Hello, World! Foo=Bar; Baz#Qux");
    EXPECT_EQ(ini2.getInt("S1.num"), INT_MAX);
    EXPECT_TRUE(ini2.getBool("S1.flag"));
}

TEST_F(IniSettingsTest, LoadCommentLines)
{
    {
        std::ofstream f(path);
        f << "; this is a comment\n"
          << "# another comment\n"
          << "[Sec]\n"
          << "key=value\n";
    }
    IniSettings ini(path.string());
    ini.load();
    EXPECT_EQ(ini.get("Sec.key"), "value");
}
