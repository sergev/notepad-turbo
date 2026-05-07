#include "util.h"
#include "SessionManager.h"
#include "IniSettings.h"

// Use a non-existent path — we never call load(), so no file I/O occurs.
static IniSettings make_ini() { return IniSettings("/tmp/session_test_no_file.ini"); }

TEST(SessionManagerTest, SaveAndLoadEmpty)
{
    auto ini = make_ini();
    SessionManager sm;
    sm.saveSession(ini, {});
    std::vector<std::string> out;
    sm.loadSession(ini, out);
    EXPECT_TRUE(out.empty());
}

TEST(SessionManagerTest, SaveAndLoadSingleFile)
{
    auto ini = make_ini();
    SessionManager sm;
    sm.saveSession(ini, {"/tmp/a.txt"});
    std::vector<std::string> out;
    sm.loadSession(ini, out);
    ASSERT_EQ(out.size(), 1u);
    EXPECT_EQ(out[0], "/tmp/a.txt");
}

TEST(SessionManagerTest, SaveAndLoadMultipleFiles)
{
    auto ini = make_ini();
    SessionManager sm;
    std::vector<std::string> files = {"/a", "/b", "/c"};
    sm.saveSession(ini, files);
    std::vector<std::string> out;
    sm.loadSession(ini, out);
    EXPECT_EQ(out, files);
}

TEST(SessionManagerTest, LoadCountZero)
{
    auto ini = make_ini();
    ini.setInt("Session.count", 0);
    SessionManager sm;
    std::vector<std::string> out;
    sm.loadSession(ini, out);
    EXPECT_TRUE(out.empty());
}

TEST(SessionManagerTest, LoadSkipsEmptyPaths)
{
    auto ini = make_ini();
    ini.setInt("Session.count", 2);
    ini.set("Session.file.0", "");
    ini.set("Session.file.1", "/valid");
    SessionManager sm;
    std::vector<std::string> out;
    sm.loadSession(ini, out);
    ASSERT_EQ(out.size(), 1u);
    EXPECT_EQ(out[0], "/valid");
}

TEST(SessionManagerTest, SaveOverwritesPrevious)
{
    auto ini = make_ini();
    SessionManager sm;
    sm.saveSession(ini, {"/a"});
    sm.saveSession(ini, {"/b", "/c"});
    std::vector<std::string> out;
    sm.loadSession(ini, out);
    ASSERT_EQ(out.size(), 2u);
    EXPECT_EQ(out[0], "/b");
    EXPECT_EQ(out[1], "/c");
}
