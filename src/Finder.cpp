/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "Finder.h"
#include <regex>
#include <algorithm>
#include <cctype>

Finder::Finder(ITextBuffer *buf) : buffer(buf) {}

void Finder::setBuffer(ITextBuffer *buf) { buffer = buf; }
void Finder::setSearchFlags(int flags)    { search_flags = flags; }
void Finder::setWrap(bool wrap_)          { wrap = wrap_; }
void Finder::setSearchText(const std::string &text_) { text = text_; }

static FindRange literalSearch(const std::string &haystack, const std::string &needle,
                               int from, int to, bool caseSensitive)
{
    if (needle.empty() || from >= to) return {};
    std::string h = haystack.substr(from, to - from);
    std::string n = needle;
    if (!caseSensitive) {
        for (char &c : h) c = (char)std::tolower((unsigned char)c);
        for (char &c : n) c = (char)std::tolower((unsigned char)c);
    }
    size_t pos = h.find(n);
    if (pos == std::string::npos) return {};
    return { from + (int)pos, from + (int)pos + (int)needle.size() };
}

static FindRange literalSearchBackward(const std::string &haystack, const std::string &needle,
                                       int from, int to, bool caseSensitive)
{
    if (needle.empty() || from >= to) return {};
    std::string h = haystack.substr(from, to - from);
    std::string n = needle;
    if (!caseSensitive) {
        for (char &c : h) c = (char)std::tolower((unsigned char)c);
        for (char &c : n) c = (char)std::tolower((unsigned char)c);
    }
    size_t pos = h.rfind(n);
    if (pos == std::string::npos) return {};
    return { from + (int)pos, from + (int)pos + (int)needle.size() };
}

FindRange Finder::searchForward(int from, int to)
{
    std::string content = buffer->flatText();
    if (to < 0 || to > (int)content.size()) to = (int)content.size();
    bool caseSens = (search_flags & FIND_MATCHCASE) != 0;

    if (search_flags & FIND_REGEXP) {
        auto flags = std::regex::ECMAScript;
        if (!caseSens) flags |= std::regex::icase;
        try {
            std::regex re(text, flags);
            auto begin = content.cbegin() + from;
            auto end   = content.cbegin() + to;
            std::smatch m;
            if (std::regex_search(begin, end, m, re)) {
                int s = (int)(m.prefix().second - content.cbegin());
                return { s, s + (int)m[0].length() };
            }
        } catch (...) {}
        return {};
    }
    return literalSearch(content, text, from, to, caseSens);
}

FindRange Finder::searchBackward(int from, int to)
{
    std::string content = buffer->flatText();
    if (to < 0 || to > (int)content.size()) to = (int)content.size();
    bool caseSens = (search_flags & FIND_MATCHCASE) != 0;
    return literalSearchBackward(content, text, from, to, caseSens);
}

FindRange Finder::findNext(int startPos)
{
    did_latest_search_wrap = false;
    if (text.empty()) return {};

    int pos = startPos < 0 ? buffer->selectionEnd() : startPos;
    FindRange r = searchForward(pos, -1);
    if (r.valid()) return r;
    if (wrap) {
        r = searchForward(0, pos);
        if (r.valid()) { did_latest_search_wrap = true; return r; }
    }
    return {};
}

FindRange Finder::findPrev()
{
    did_latest_search_wrap = false;
    if (text.empty()) return {};

    int pos = buffer->selectionStart();
    FindRange r = searchBackward(0, pos);
    if (r.valid()) return r;
    if (wrap) {
        r = searchBackward(pos, -1);
        if (r.valid()) { did_latest_search_wrap = true; return r; }
    }
    return {};
}

int Finder::count()
{
    int total = 0;
    forEachMatch([&](int, int end) { total++; return end; });
    return total;
}

void Finder::forEachMatch(std::function<int(int start, int end)> callback)
{
    std::string content = buffer->flatText();
    int len = (int)content.size();
    bool caseSens = (search_flags & FIND_MATCHCASE) != 0;

    if (search_flags & FIND_REGEXP) {
        auto flags = std::regex::ECMAScript;
        if (!caseSens) flags |= std::regex::icase;
        try {
            std::regex re(text, flags);
            auto it  = std::sregex_iterator(content.begin(), content.end(), re);
            auto end = std::sregex_iterator();
            for (; it != end; ++it) {
                int s = (int)it->position();
                int e = s + (int)(*it)[0].length();
                int next = callback(s, e);
                if (next <= s) break;
            }
        } catch (...) {}
        return;
    }

    int pos = 0;
    while (pos < len) {
        FindRange r = literalSearch(content, text, pos, len, caseSens);
        if (!r.valid()) break;
        int next = callback(r.start, r.end);
        if (next <= r.start) break;
        pos = next;
    }
}

FindRange Finder::replaceSelectionIfMatch(const std::string &replaceText)
{
    int selStart = buffer->selectionStart();
    int selEnd   = buffer->selectionEnd();
    std::string content = buffer->flatText();
    bool caseSens = (search_flags & FIND_MATCHCASE) != 0;

    FindRange r = literalSearch(content, text, selStart, selEnd, caseSens);
    if (!r.valid() || r.start != selStart || r.end != selEnd) return {};

    buffer->replaceSelection(replaceText);
    return { selStart, selStart + (int)replaceText.size() };
}

int Finder::replaceAll(const std::string &replaceText)
{
    if (text.empty()) return 0;

    std::string content = buffer->flatText();
    bool caseSens = (search_flags & FIND_MATCHCASE) != 0;
    std::string result;
    int total = 0;
    int pos = 0;
    int len = (int)content.size();

    if (search_flags & FIND_REGEXP) {
        auto flags = std::regex::ECMAScript;
        if (!caseSens) flags |= std::regex::icase;
        try {
            std::regex re(text, flags);
            total = (int)std::distance(
                std::sregex_iterator(content.begin(), content.end(), re),
                std::sregex_iterator());
            result = std::regex_replace(content, re, replaceText);
        } catch (...) { return 0; }
    } else {
        while (pos < len) {
            FindRange r = literalSearch(content, text, pos, len, caseSens);
            if (!r.valid()) { result += content.substr(pos); break; }
            result += content.substr(pos, r.start - pos);
            result += replaceText;
            pos = r.end;
            total++;
        }
        if (pos >= len && total == 0) return 0;
    }

    if (total > 0) buffer->replaceAll(result);
    return total;
}
