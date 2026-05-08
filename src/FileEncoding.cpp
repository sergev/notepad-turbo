/*
 * This file is part of Notepad Turbo.
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "FileEncoding.h"

#include <uchardet.h>

#include <iconv.h>

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cctype>
#include <cstring>
#include <initializer_list>
#include <optional>
#include <string>

namespace {

bool startsWith(const std::string &bytes, std::initializer_list<unsigned char> prefix)
{
    if (bytes.size() < prefix.size())
        return false;

    size_t index = 0;
    for (unsigned char ch : prefix) {
        if (static_cast<unsigned char>(bytes[index]) != ch)
            return false;
        ++index;
    }
    return true;
}

bool isValidUtf8(const std::string &bytes)
{
    size_t i = 0;
    while (i < bytes.size()) {
        const unsigned char ch = static_cast<unsigned char>(bytes[i]);
        if (ch < 0x80) {
            ++i;
            continue;
        }

        size_t extra = 0;
        unsigned int value = 0;
        if ((ch & 0xE0) == 0xC0) {
            extra = 1;
            value = ch & 0x1F;
            if (value == 0)
                return false;
        } else if ((ch & 0xF0) == 0xE0) {
            extra = 2;
            value = ch & 0x0F;
        } else if ((ch & 0xF8) == 0xF0) {
            extra = 3;
            value = ch & 0x07;
        } else {
            return false;
        }

        if (i + extra >= bytes.size())
            return false;

        for (size_t j = 1; j <= extra; ++j) {
            const unsigned char trail = static_cast<unsigned char>(bytes[i + j]);
            if ((trail & 0xC0) != 0x80)
                return false;
            value = (value << 6) | (trail & 0x3F);
        }

        if ((extra == 2 && value < 0x800) ||
            (extra == 3 && value < 0x10000) ||
            value > 0x10FFFF ||
            (value >= 0xD800 && value <= 0xDFFF))
            return false;

        i += extra + 1;
    }
    return true;
}

std::string normalizedCharset(std::string charset)
{
    std::transform(charset.begin(), charset.end(), charset.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });

    if (charset == "ASCII" || charset == "US-ASCII")
        return "UTF-8";
    if (charset == "UTF8")
        return "UTF-8";
    if (charset == "UTF-16")
        return "UTF-16";
    if (charset == "UTF-16LE")
        return "UTF-16LE";
    if (charset == "UTF-16BE")
        return "UTF-16BE";

    return charset;
}

size_t utf8CharLength(unsigned char ch)
{
    if (ch < 0x80)
        return 1;
    if ((ch & 0xE0) == 0xC0)
        return 2;
    if ((ch & 0xF0) == 0xE0)
        return 3;
    if ((ch & 0xF8) == 0xF0)
        return 4;
    return 1;
}

std::optional<std::string> convertEncoding(const std::string &bytes,
                                           const std::string &fromCharset,
                                           const std::string &toCharset,
                                           bool replaceErrors)
{
    iconv_t cd = iconv_open(toCharset.c_str(), fromCharset.c_str());
    if (cd == reinterpret_cast<iconv_t>(-1))
        return std::nullopt;

    std::string output;
    output.resize(bytes.size() * 4 + 16);

    char *input = const_cast<char *>(bytes.data());
    size_t inputLeft = bytes.size();
    char *out = output.data();
    size_t outputLeft = output.size();
    std::optional<std::string> replacement;

    auto growOutput = [&]() {
        const size_t used = output.size() - outputLeft;
        output.resize(output.size() * 2);
        out = output.data() + used;
        outputLeft = output.size() - used;
    };

    while (inputLeft > 0) {
        if (iconv(cd, &input, &inputLeft, &out, &outputLeft) != static_cast<size_t>(-1))
            continue;

        if ((errno == EILSEQ || errno == EINVAL) && replaceErrors) {
            if (!replacement)
                replacement = convertEncoding("?", "UTF-8", toCharset, false);
            if (!replacement) {
                iconv_close(cd);
                return std::nullopt;
            }

            while (outputLeft < replacement->size())
                growOutput();

            memcpy(out, replacement->data(), replacement->size());
            out += replacement->size();
            outputLeft -= replacement->size();

            size_t skip = (fromCharset == "UTF-8")
                ? std::min(utf8CharLength(static_cast<unsigned char>(*input)), inputLeft)
                : 1;
            input += skip;
            inputLeft -= skip;
            continue;
        }

        if (errno != E2BIG) {
            iconv_close(cd);
            return std::nullopt;
        }

        growOutput();
    }

    while (iconv(cd, nullptr, nullptr, &out, &outputLeft) == static_cast<size_t>(-1)) {
        if (errno != E2BIG) {
            iconv_close(cd);
            return std::nullopt;
        }

        growOutput();
    }

    output.resize(output.size() - outputLeft);
    iconv_close(cd);
    return output;
}

std::optional<std::string> convertToUtf8(const std::string &bytes, const std::string &charset)
{
    return convertEncoding(bytes, charset, "UTF-8", false);
}

std::optional<std::string> convertFromUtf8(const std::string &utf8,
                                           const std::string &charset,
                                           bool replaceErrors)
{
    return convertEncoding(utf8, "UTF-8", charset, replaceErrors);
}

std::string detectCharset(const std::string &bytes)
{
    uchardet_t detector = uchardet_new();
    if (!detector)
        return {};

    std::string charset;
    if (uchardet_handle_data(detector, bytes.data(), bytes.size()) == 0) {
        uchardet_data_end(detector);
        if (const char *detected = uchardet_get_charset(detector))
            charset = detected;
    }
    uchardet_delete(detector);
    return normalizedCharset(charset);
}

} // namespace

namespace FileEncoding {

DecodeResult decode(const std::string &bytes)
{
    DecodeResult result;
    result.source.originalBytes = bytes;

    if (bytes.empty()) {
        result.source.charset = "UTF-8";
        result.utf8 = bytes;
        return result;
    }

    if (startsWith(bytes, { 0xEF, 0xBB, 0xBF })) {
        std::string withoutBom = bytes.substr(3);
        if (isValidUtf8(withoutBom)) {
            result.source.charset = "UTF-8";
            result.source.bom = BomKind::Utf8;
            result.utf8 = withoutBom;
            return result;
        }
        result.source.decoded = false;
        result.utf8 = bytes;
        return result;
    }

    if (startsWith(bytes, { 0xFF, 0xFE })) {
        std::string withoutBom = bytes.substr(2);
        if (auto converted = convertToUtf8(withoutBom, "UTF-16LE")) {
            result.source.charset = "UTF-16LE";
            result.source.bom = BomKind::Utf16Le;
            result.utf8 = *converted;
            return result;
        }
        result.source.decoded = false;
        result.utf8 = bytes;
        return result;
    }

    if (startsWith(bytes, { 0xFE, 0xFF })) {
        std::string withoutBom = bytes.substr(2);
        if (auto converted = convertToUtf8(withoutBom, "UTF-16BE")) {
            result.source.charset = "UTF-16BE";
            result.source.bom = BomKind::Utf16Be;
            result.utf8 = *converted;
            return result;
        }
        result.source.decoded = false;
        result.utf8 = bytes;
        return result;
    }

    if (isValidUtf8(bytes)) {
        result.source.charset = "UTF-8";
        result.utf8 = bytes;
        return result;
    }

    std::string charset = detectCharset(bytes);
    if (!charset.empty() && charset != "UTF-8") {
        if (auto converted = convertToUtf8(bytes, charset)) {
            result.source.charset = charset;
            result.utf8 = *converted;
            return result;
        }
    }

    result.source.decoded = false;
    result.utf8 = bytes;
    return result;
}

std::string decodeToUtf8(const std::string &bytes)
{
    return decode(bytes).utf8;
}

EncodeResult encodeFromUtf8(const std::string &utf8,
                            const SourceEncoding &source,
                            SavePolicy policy)
{
    EncodeResult result;
    result.source = source;

    if (!source.decoded) {
        if (utf8 == source.originalBytes) {
            result.ok = true;
            result.bytes = source.originalBytes;
            return result;
        }
        if (policy == SavePolicy::Fail)
            return result;

        result.ok = true;
        result.bytes = utf8;
        result.source = SourceEncoding{};
        return result;
    }

    SourceEncoding target = source;
    if (policy == SavePolicy::Utf8)
        target = SourceEncoding{};

    std::string body;
    if (target.charset == "UTF-8") {
        body = utf8;
    } else {
        bool replaceErrors = (policy == SavePolicy::Replace);
        auto converted = convertFromUtf8(utf8, target.charset, replaceErrors);
        if (!converted) {
            if (policy == SavePolicy::Utf8) {
                body = utf8;
                target = SourceEncoding{};
            } else {
                return result;
            }
        } else {
            body = *converted;
        }
    }

    switch (target.bom) {
        case BomKind::Utf8:
            result.bytes.assign("\xEF\xBB\xBF", 3);
            break;
        case BomKind::Utf16Le:
            result.bytes.assign("\xFF\xFE", 2);
            break;
        case BomKind::Utf16Be:
            result.bytes.assign("\xFE\xFF", 2);
            break;
        case BomKind::None:
            break;
    }

    result.bytes += body;
    result.source = target;
    result.ok = true;
    return result;
}

SavePolicy parseSavePolicy(const std::string &value)
{
    if (value == "fail")
        return SavePolicy::Fail;
    if (value == "utf8")
        return SavePolicy::Utf8;
    return SavePolicy::Replace;
}

const char *savePolicyName(SavePolicy policy)
{
    switch (policy) {
        case SavePolicy::Fail:
            return "fail";
        case SavePolicy::Utf8:
            return "utf8";
        case SavePolicy::Replace:
            return "replace";
    }
    return DefaultSavePolicyName;
}

} // namespace FileEncoding
