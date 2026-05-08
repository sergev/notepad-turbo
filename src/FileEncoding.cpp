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

std::optional<std::string> convertToUtf8(const std::string &bytes, const std::string &charset)
{
    iconv_t cd = iconv_open("UTF-8", charset.c_str());
    if (cd == reinterpret_cast<iconv_t>(-1))
        return std::nullopt;

    std::string output;
    output.resize(bytes.size() * 4 + 16);

    char *input = const_cast<char *>(bytes.data());
    size_t inputLeft = bytes.size();
    char *out = output.data();
    size_t outputLeft = output.size();

    while (inputLeft > 0) {
        if (iconv(cd, &input, &inputLeft, &out, &outputLeft) != static_cast<size_t>(-1))
            continue;

        if (errno != E2BIG) {
            iconv_close(cd);
            return std::nullopt;
        }

        const size_t used = output.size() - outputLeft;
        output.resize(output.size() * 2);
        out = output.data() + used;
        outputLeft = output.size() - used;
    }

    while (iconv(cd, nullptr, nullptr, &out, &outputLeft) == static_cast<size_t>(-1)) {
        if (errno != E2BIG) {
            iconv_close(cd);
            return std::nullopt;
        }

        const size_t used = output.size() - outputLeft;
        output.resize(output.size() * 2);
        out = output.data() + used;
        outputLeft = output.size() - used;
    }

    output.resize(output.size() - outputLeft);
    iconv_close(cd);
    return output;
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

std::string decodeToUtf8(const std::string &bytes)
{
    if (bytes.empty())
        return bytes;

    if (startsWith(bytes, { 0xEF, 0xBB, 0xBF })) {
        std::string withoutBom = bytes.substr(3);
        return isValidUtf8(withoutBom) ? withoutBom : bytes;
    }

    if (startsWith(bytes, { 0xFF, 0xFE })) {
        std::string withoutBom = bytes.substr(2);
        if (auto converted = convertToUtf8(withoutBom, "UTF-16LE"))
            return *converted;
        return bytes;
    }

    if (startsWith(bytes, { 0xFE, 0xFF })) {
        std::string withoutBom = bytes.substr(2);
        if (auto converted = convertToUtf8(withoutBom, "UTF-16BE"))
            return *converted;
        return bytes;
    }

    if (isValidUtf8(bytes))
        return bytes;

    std::string charset = detectCharset(bytes);
    if (charset.empty() || charset == "UTF-8")
        return bytes;

    if (auto converted = convertToUtf8(bytes, charset))
        return *converted;

    return bytes;
}

} // namespace FileEncoding
