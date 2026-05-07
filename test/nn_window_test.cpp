#include "nn_test_helpers.h"

// Test 6: NNWindow::getTitle() includes '*' prefix when editor is modified.
TEST(NNWindow, TitleShowsAsteriskWhenModified)
{
    NNWindowTest ctx;
    ctx.init(42, 12, "notes.txt");
    NNEditor *ed = ctx.editor();

    ed->modified = False;
    std::string title = ctx.win->getTitle(80);
    EXPECT_EQ(title.find('*'), std::string::npos)
        << "Unmodified window title should not contain '*'";

    ed->modified = True;
    title = ctx.win->getTitle(80);
    EXPECT_FALSE(title.empty());
    EXPECT_EQ(title[0], '*')
        << "Modified window title should start with '*'";
}

// Extra: title contains the file name in both states.
TEST(NNWindow, TitleContainsFileName)
{
    NNWindowTest ctx;
    ctx.init(42, 12, "myfile.cpp");
    NNEditor *ed = ctx.editor();

    ed->modified = False;
    EXPECT_NE(std::string(ctx.win->getTitle(80)).find("myfile.cpp"),
              std::string::npos);

    ed->modified = True;
    EXPECT_NE(std::string(ctx.win->getTitle(80)).find("myfile.cpp"),
              std::string::npos);
}
