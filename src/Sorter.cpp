/*
 * This file is part of Notepad Turbo.
 * Copyright 2026 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "Sorter.h"
#include <algorithm>
#include <cctype>

static int caseInsensitiveCompare(std::string_view a, std::string_view b) {
    size_t n = std::min(a.size(), b.size());
    for (size_t i = 0; i < n; ++i) {
        int ca = std::tolower((unsigned char)a[i]);
        int cb = std::tolower((unsigned char)b[i]);
        if (ca != cb) return ca - cb;
    }
    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return 1;
    return 0;
}

void CaseInsensitiveSorter::sort(std::vector<std::string_view> &lines) const
{
    std::stable_sort(lines.begin(), lines.end(),
        [dir = direction](std::string_view a, std::string_view b) {
            int result = caseInsensitiveCompare(a, b);
            return dir == Direction::Ascending ? (result < 0) : (result > 0);
        });
}

void CaseSensitiveSorter::sort(std::vector<std::string_view> &lines) const
{
    std::stable_sort(lines.begin(), lines.end(),
        [dir = direction](std::string_view a, std::string_view b) {
            int result = a.compare(b);
            return dir == Direction::Ascending ? (result < 0) : (result > 0);
        });
}

void LineLengthSorter::sort(std::vector<std::string_view> &lines) const
{
    std::stable_sort(lines.begin(), lines.end(),
        [dir = direction](std::string_view a, std::string_view b) {
            return dir == Direction::Ascending ? (a.size() < b.size()) : (a.size() > b.size());
        });
}

void ReverseSorter::sort(std::vector<std::string_view> &lines) const
{
    std::reverse(lines.begin(), lines.end());
}
