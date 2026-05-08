/*
 * This file is part of Notepad Turbo.
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>

namespace FileEncoding {

enum class BomKind {
    None,
    Utf8,
    Utf16Le,
    Utf16Be,
};

enum class SavePolicy {
    Fail,
    Replace,
    Utf8,
};

struct SourceEncoding {
    std::string charset = "UTF-8";
    BomKind bom = BomKind::None;
    bool decoded = true;
    std::string originalBytes;
};

struct DecodeResult {
    std::string utf8;
    SourceEncoding source;
};

struct EncodeResult {
    bool ok = false;
    std::string bytes;
    SourceEncoding source;
};

constexpr const char *DefaultSavePolicyName = "replace";

DecodeResult decode(const std::string &bytes);
std::string decodeToUtf8(const std::string &bytes);
EncodeResult encodeFromUtf8(const std::string &utf8,
                            const SourceEncoding &source,
                            SavePolicy policy);

SavePolicy parseSavePolicy(const std::string &value);
const char *savePolicyName(SavePolicy policy);

}
