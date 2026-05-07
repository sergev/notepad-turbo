#include "util.h"
#include "Finder.h"

struct MockTextBuffer : ITextBuffer {
    std::string content;
    int selStart = 0;
    int selEnd   = 0;

    std::string flatText() const override { return content; }
    int selectionStart() const override { return selStart; }
    int selectionEnd()   const override { return selEnd; }

    void replaceSelection(const std::string &text) override {
        content = content.substr(0, selStart) + text + content.substr(selEnd);
        selEnd = selStart + (int)text.size();
    }
    void replaceAll(const std::string &text) override { content = text; }
};

class FinderTest : public ::testing::Test {
protected:
    MockTextBuffer buf;
    Finder finder{&buf};
};

// ---- Literal forward search ----

TEST_F(FinderTest, FindNextBasic)
{
    buf.content = "hello world";
    finder.setSearchText("world");
    FindRange r = finder.findNext(0);
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 6);
    EXPECT_EQ(r.end, 11);
}

TEST_F(FinderTest, FindNextFromPosition)
{
    buf.content = "abcabc";
    finder.setSearchText("bc");
    FindRange r = finder.findNext(3);
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 4);
    EXPECT_EQ(r.end, 6);
}

TEST_F(FinderTest, FindNextNotFound)
{
    buf.content = "hello";
    finder.setSearchText("xyz");
    FindRange r = finder.findNext(0);
    EXPECT_FALSE(r.valid());
}

TEST_F(FinderTest, FindNextWraps)
{
    buf.content = "abc";
    buf.selEnd = 3;  // cursor at end
    finder.setSearchText("a");
    finder.setWrap(true);
    FindRange r = finder.findNext();
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 0);
    EXPECT_TRUE(finder.didLatestSearchWrapAround());
}

TEST_F(FinderTest, FindNextNoWrapNoResult)
{
    buf.content = "abc";
    buf.selEnd = 3;
    finder.setSearchText("a");
    finder.setWrap(false);
    FindRange r = finder.findNext();
    EXPECT_FALSE(r.valid());
    EXPECT_FALSE(finder.didLatestSearchWrapAround());
}

// ---- Backward search ----

TEST_F(FinderTest, FindPrevBasic)
{
    buf.content = "aXbXc";
    buf.selStart = 3;  // cursor just before second X; searches [0,3) → finds first X
    finder.setSearchText("X");
    FindRange r = finder.findPrev();
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 1);
    EXPECT_EQ(r.end, 2);
}

TEST_F(FinderTest, FindPrevWraps)
{
    buf.content = "abXcd";
    buf.selStart = 0;
    finder.setSearchText("X");
    finder.setWrap(true);
    FindRange r = finder.findPrev();
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 2);
    EXPECT_TRUE(finder.didLatestSearchWrapAround());
}

// ---- Count ----

TEST_F(FinderTest, CountMatches)
{
    buf.content = "abcabc";
    finder.setSearchText("a");
    EXPECT_EQ(finder.count(), 2);
}

TEST_F(FinderTest, CountNoMatches)
{
    buf.content = "hello";
    finder.setSearchText("xyz");
    EXPECT_EQ(finder.count(), 0);
}

// ---- Case sensitivity ----

TEST_F(FinderTest, CaseSensitiveMismatch)
{
    buf.content = "hello world";
    finder.setSearchFlags(FIND_MATCHCASE);
    finder.setSearchText("Hello");
    EXPECT_FALSE(finder.findNext(0).valid());
}

TEST_F(FinderTest, CaseInsensitiveMatch)
{
    buf.content = "hello world";
    finder.setSearchFlags(0);
    finder.setSearchText("HELLO");
    FindRange r = finder.findNext(0);
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 0);
    EXPECT_EQ(r.end, 5);
}

// ---- Regex ----

TEST_F(FinderTest, RegexFindBasic)
{
    buf.content = "abc 123 def";
    finder.setSearchFlags(FIND_REGEXP);
    finder.setSearchText("\\d+");
    FindRange r = finder.findNext(0);
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(r.start, 4);
    EXPECT_EQ(r.end, 7);
}

TEST_F(FinderTest, RegexFindCaseInsensitive)
{
    buf.content = "ABC";
    finder.setSearchFlags(FIND_REGEXP);
    finder.setSearchText("[a-z]+");
    FindRange r = finder.findNext(0);
    EXPECT_TRUE(r.valid());
}

TEST_F(FinderTest, RegexInvalidPattern)
{
    buf.content = "hello";
    finder.setSearchFlags(FIND_REGEXP);
    finder.setSearchText("[invalid");
    EXPECT_NO_THROW({
        FindRange r = finder.findNext(0);
        EXPECT_FALSE(r.valid());
    });
}

// ---- Replace ----

TEST_F(FinderTest, ReplaceSelectionIfMatchSuccess)
{
    buf.content = "hello";
    buf.selStart = 0;
    buf.selEnd   = 5;
    finder.setSearchText("hello");
    FindRange r = finder.replaceSelectionIfMatch("world");
    EXPECT_TRUE(r.valid());
    EXPECT_EQ(buf.content, "world");
}

TEST_F(FinderTest, ReplaceSelectionIfMatchNoMatch)
{
    buf.content = "hello";
    buf.selStart = 0;
    buf.selEnd   = 5;
    finder.setSearchText("xyz");
    FindRange r = finder.replaceSelectionIfMatch("world");
    EXPECT_FALSE(r.valid());
    EXPECT_EQ(buf.content, "hello");
}

TEST_F(FinderTest, ReplaceAllLiteral)
{
    buf.content = "aXbXcX";
    finder.setSearchText("X");
    int count = finder.replaceAll("_");
    EXPECT_EQ(count, 3);
    EXPECT_EQ(buf.content, "a_b_c_");
}

TEST_F(FinderTest, ReplaceAllRegex)
{
    buf.content = "a1b2c3";
    finder.setSearchFlags(FIND_REGEXP);
    finder.setSearchText("\\d");
    int count = finder.replaceAll("#");
    EXPECT_EQ(count, 3);
    EXPECT_EQ(buf.content, "a#b#c#");
}

TEST_F(FinderTest, ReplaceAllNoneFound)
{
    buf.content = "hello";
    finder.setSearchText("xyz");
    int count = finder.replaceAll("replaced");
    EXPECT_EQ(count, 0);
    EXPECT_EQ(buf.content, "hello");
}

TEST_F(FinderTest, ReplaceAllEmptySearchText)
{
    buf.content = "hello";
    finder.setSearchText("");
    int count = finder.replaceAll("x");
    EXPECT_EQ(count, 0);
    EXPECT_EQ(buf.content, "hello");
}

// ---- Edge cases ----

TEST_F(FinderTest, EmptySearchTextFindNext)
{
    buf.content = "hello";
    finder.setSearchText("");
    EXPECT_FALSE(finder.findNext(0).valid());
}

TEST_F(FinderTest, FindInEmptyContent)
{
    buf.content = "";
    finder.setSearchText("x");
    EXPECT_FALSE(finder.findNext(0).valid());
    EXPECT_FALSE(finder.findPrev().valid());
}
