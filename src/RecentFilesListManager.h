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
#include <vector>

class IniSettings;

class RecentFilesListManager {
public:
    static constexpr int MaxFiles = 15;

    explicit RecentFilesListManager(IniSettings *settings);

    void load();
    void save() const;

    void addFile(const std::string &filePath);
    void removeFile(const std::string &filePath);
    void clear();

    std::string mostRecentFile() const;
    const std::vector<std::string> &fileList() const { return recentFiles; }
    int count() const { return (int)recentFiles.size(); }

private:
    IniSettings *settings;
    std::vector<std::string> recentFiles;
};
