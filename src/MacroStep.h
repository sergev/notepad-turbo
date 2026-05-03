/*
 * This file is part of Notepad Turbo.
 * Copyright 2022 Justin Dailey
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <cstdint>

enum class NNMacroCmd : uint32_t {
    InsertText,
    Delete,
    CharLeft,
    CharRight,
    LineUp,
    LineDown,
    WordLeft,
    WordRight,
    LineStart,
    LineEnd,
    SelectAll,
    PageUp,
    PageDown,
    Cut,
    Copy,
    Paste,
    DeleteLine,
    FindNext,
    FindPrev,
    ReplaceOnce,
    ReplaceAll,
};

struct NNMacroStep {
    NNMacroCmd cmd;
    std::string text;

    NNMacroStep(NNMacroCmd cmd_, const std::string &text_ = {})
        : cmd(cmd_), text(text_) {}
};
