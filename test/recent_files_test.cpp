#include "util.h"
#include "RecentFilesListManager.h"
#include "IniSettings.h"

#include <filesystem>

namespace fs = std::filesystem;

static fs::path tmp_path()
{
    return fs::temp_directory_path() / ("nt_recent_" + get_test_name() + ".ini");
}

class RecentFilesTest : public ::testing::Test {
protected:
    void TearDown() override { fs::remove(path); }
    fs::path path = tmp_path();
    IniSettings ini{path.string()};
    RecentFilesListManager mgr{&ini};
};

TEST_F(RecentFilesTest, InitiallyEmpty)
{
    EXPECT_EQ(mgr.count(), 0);
    EXPECT_EQ(mgr.mostRecentFile(), "");
}

TEST_F(RecentFilesTest, AddSingleFile)
{
    mgr.addFile("/a.txt");
    EXPECT_EQ(mgr.count(), 1);
    EXPECT_EQ(mgr.mostRecentFile(), "/a.txt");
}

TEST_F(RecentFilesTest, AddPromotesExisting)
{
    mgr.addFile("/a");
    mgr.addFile("/b");
    mgr.addFile("/a");
    EXPECT_EQ(mgr.count(), 2);
    EXPECT_EQ(mgr.mostRecentFile(), "/a");
    EXPECT_EQ(mgr.fileList()[1], "/b");
}

TEST_F(RecentFilesTest, AddEnforcesMaxFiles)
{
    for (int i = 0; i < RecentFilesListManager::MaxFiles + 1; ++i)
        mgr.addFile("/file" + std::to_string(i));
    EXPECT_EQ(mgr.count(), RecentFilesListManager::MaxFiles);
    // Newest file is at front
    EXPECT_EQ(mgr.mostRecentFile(), "/file" + std::to_string(RecentFilesListManager::MaxFiles));
    // Oldest (file0) was dropped
    const auto &list = mgr.fileList();
    EXPECT_TRUE(std::find(list.begin(), list.end(), "/file0") == list.end());
}

TEST_F(RecentFilesTest, RemoveFile)
{
    mgr.addFile("/a");
    mgr.addFile("/b");
    mgr.addFile("/c");
    mgr.removeFile("/b");
    EXPECT_EQ(mgr.count(), 2);
    const auto &list = mgr.fileList();
    EXPECT_TRUE(std::find(list.begin(), list.end(), "/b") == list.end());
    EXPECT_EQ(list[0], "/c");
    EXPECT_EQ(list[1], "/a");
}

TEST_F(RecentFilesTest, RemoveNonexistentIsNoOp)
{
    mgr.addFile("/a");
    EXPECT_NO_THROW(mgr.removeFile("/nonexistent"));
    EXPECT_EQ(mgr.count(), 1);
}

TEST_F(RecentFilesTest, Clear)
{
    mgr.addFile("/a");
    mgr.addFile("/b");
    mgr.clear();
    EXPECT_EQ(mgr.count(), 0);
    EXPECT_EQ(mgr.mostRecentFile(), "");
}

TEST_F(RecentFilesTest, SaveAndLoad)
{
    mgr.addFile("/first");
    mgr.addFile("/second");
    mgr.save();
    ini.save();  // flush in-memory IniSettings to disk

    IniSettings ini2(path.string());
    ini2.load();
    RecentFilesListManager mgr2(&ini2);
    mgr2.load();

    ASSERT_EQ(mgr2.count(), 2);
    EXPECT_EQ(mgr2.mostRecentFile(), "/second");
    EXPECT_EQ(mgr2.fileList()[1], "/first");
}
