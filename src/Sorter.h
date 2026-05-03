/*
 * This file is part of Notepad Turbo.
 * Copyright 2026 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <vector>
#include <string_view>

class Sorter
{
public:
    enum class Direction {
        Ascending,
        Descending
    };

    explicit Sorter(Direction direction) : direction(direction) {}
    virtual ~Sorter() = default;

    virtual void sort(std::vector<std::string_view> &lines) const = 0;

protected:
    Direction direction;
};


class CaseInsensitiveSorter : public Sorter
{
public:
    using Sorter::Sorter;
    void sort(std::vector<std::string_view> &lines) const override;
};


class CaseSensitiveSorter : public Sorter
{
public:
    using Sorter::Sorter;
    void sort(std::vector<std::string_view> &lines) const override;
};


class LineLengthSorter : public Sorter
{
public:
    using Sorter::Sorter;
    void sort(std::vector<std::string_view> &lines) const override;
};


class ReverseSorter : public Sorter
{
public:
    using Sorter::Sorter;
    void sort(std::vector<std::string_view> &lines) const override;
};
