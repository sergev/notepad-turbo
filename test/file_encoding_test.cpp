#include "FileEncoding.h"

#include <gtest/gtest.h>

#include <string>

TEST(FileEncoding, Utf8BomRoundTrips)
{
    FileEncoding::DecodeResult decoded = FileEncoding::decode("\xEF\xBB\xBFhello\n");

    EXPECT_EQ(decoded.utf8, "hello\n");
    EXPECT_EQ(decoded.source.charset, "UTF-8");
    EXPECT_EQ(decoded.source.bom, FileEncoding::BomKind::Utf8);

    FileEncoding::EncodeResult encoded =
        FileEncoding::encodeFromUtf8(decoded.utf8, decoded.source, FileEncoding::SavePolicy::Fail);

    ASSERT_TRUE(encoded.ok);
    EXPECT_EQ(encoded.bytes, "\xEF\xBB\xBFhello\n");
}

TEST(FileEncoding, Utf16BeBomRoundTrips)
{
    const char raw[] = {
        static_cast<char>(0xFE), static_cast<char>(0xFF),
        0, 'h', 0, 'i', 0, '\n',
    };
    FileEncoding::DecodeResult decoded = FileEncoding::decode(std::string(raw, sizeof(raw)));

    EXPECT_EQ(decoded.utf8, "hi\n");
    EXPECT_EQ(decoded.source.charset, "UTF-16BE");
    EXPECT_EQ(decoded.source.bom, FileEncoding::BomKind::Utf16Be);

    FileEncoding::EncodeResult encoded =
        FileEncoding::encodeFromUtf8(decoded.utf8, decoded.source, FileEncoding::SavePolicy::Fail);

    ASSERT_TRUE(encoded.ok);
    EXPECT_EQ(encoded.bytes, std::string(raw, sizeof(raw)));
}

TEST(FileEncoding, Windows1252UnrepresentablePolicies)
{
    FileEncoding::SourceEncoding source;
    source.charset = "WINDOWS-1252";

    const std::string text = "snowman \xE2\x98\x83\n";

    FileEncoding::EncodeResult failed =
        FileEncoding::encodeFromUtf8(text, source, FileEncoding::SavePolicy::Fail);
    EXPECT_FALSE(failed.ok);

    FileEncoding::EncodeResult replaced =
        FileEncoding::encodeFromUtf8(text, source, FileEncoding::SavePolicy::Replace);
    ASSERT_TRUE(replaced.ok);
    EXPECT_EQ(replaced.bytes, "snowman ?\n");

    FileEncoding::EncodeResult utf8 =
        FileEncoding::encodeFromUtf8(text, source, FileEncoding::SavePolicy::Utf8);
    ASSERT_TRUE(utf8.ok);
    EXPECT_EQ(utf8.bytes, text);
    EXPECT_EQ(utf8.source.charset, "UTF-8");
}

TEST(FileEncoding, SavePolicyParsingDefaultsToReplace)
{
    EXPECT_EQ(FileEncoding::parseSavePolicy("fail"), FileEncoding::SavePolicy::Fail);
    EXPECT_EQ(FileEncoding::parseSavePolicy("utf8"), FileEncoding::SavePolicy::Utf8);
    EXPECT_EQ(FileEncoding::parseSavePolicy("replace"), FileEncoding::SavePolicy::Replace);
    EXPECT_EQ(FileEncoding::parseSavePolicy("bogus"), FileEncoding::SavePolicy::Replace);
}
