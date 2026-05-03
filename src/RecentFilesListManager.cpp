/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "RecentFilesListManager.h"
#include "IniSettings.h"

#include <algorithm>

RecentFilesListManager::RecentFilesListManager(IniSettings *settings)
    : settings(settings)
{
}

void RecentFilesListManager::load()
{
    recentFiles.clear();
    int count = settings->getInt("RecentFiles.count", 0);
    for (int i = 0; i < count && i < MaxFiles; ++i) {
        std::string key = "RecentFiles.file." + std::to_string(i);
        std::string path = settings->get(key);
        if (!path.empty())
            recentFiles.push_back(path);
    }
}

void RecentFilesListManager::save() const
{
    settings->removeSection("RecentFiles");
    settings->setInt("RecentFiles.count", (int)recentFiles.size());
    for (int i = 0; i < (int)recentFiles.size(); ++i)
        settings->set("RecentFiles.file." + std::to_string(i), recentFiles[i]);
}

void RecentFilesListManager::addFile(const std::string &filePath)
{
    auto it = std::find(recentFiles.begin(), recentFiles.end(), filePath);
    if (it != recentFiles.end())
        recentFiles.erase(it);
    recentFiles.insert(recentFiles.begin(), filePath);
    if ((int)recentFiles.size() > MaxFiles)
        recentFiles.resize(MaxFiles);
}

void RecentFilesListManager::removeFile(const std::string &filePath)
{
    auto it = std::find(recentFiles.begin(), recentFiles.end(), filePath);
    if (it != recentFiles.end())
        recentFiles.erase(it);
}

void RecentFilesListManager::clear()
{
    recentFiles.clear();
}

std::string RecentFilesListManager::mostRecentFile() const
{
    return recentFiles.empty() ? std::string{} : recentFiles.front();
}
