/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <functional>

// Search flags (matching Scintilla SCFIND_* values)
static constexpr int FIND_MATCHCASE   = 0x04;
static constexpr int FIND_WHOLEWORD   = 0x02;
static constexpr int FIND_WORDSTART   = 0x01;
static constexpr int FIND_REGEXP      = 0x200000;

struct FindRange {
    int start = -1;
    int end   = -1;
    bool valid() const { return start >= 0; }
};

class NNEditor;

class Finder
{
public:
    explicit Finder(NNEditor *edit);

    void setEditor(NNEditor *editor);
    void setSearchFlags(int flags);
    void setWrap(bool wrap);
    void setSearchText(const std::string &text);

    FindRange findNext(int startPos = -1);
    FindRange findPrev();
    int count();

    bool didLatestSearchWrapAround() const { return did_latest_search_wrap; }

    FindRange replaceSelectionIfMatch(const std::string &replaceText);
    int replaceAll(const std::string &replaceText);

    void forEachMatch(std::function<int(int start, int end)> callback);

private:
    FindRange searchForward(int from, int to);
    FindRange searchBackward(int from, int to);

    NNEditor *editor;
    bool did_latest_search_wrap = false;
    bool wrap = false;
    int search_flags = 0;
    std::string text;
};
