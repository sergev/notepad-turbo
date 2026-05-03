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

#include "SessionManager.h"
#include "IniSettings.h"

void SessionManager::loadSession(IniSettings &ini, std::vector<std::string> &outFiles)
{
    int count = ini.getInt("Session.count", 0);
    outFiles.reserve(count);
    for (int i = 0; i < count; ++i) {
        std::string key = "Session.file." + std::to_string(i);
        std::string path = ini.get(key);
        if (!path.empty()) {
            outFiles.push_back(path);
        }
    }
}

void SessionManager::saveSession(IniSettings &ini, const std::vector<std::string> &openFiles)
{
    ini.setInt("Session.count", (int)openFiles.size());
    for (int i = 0; i < (int)openFiles.size(); ++i) {
        std::string key = "Session.file." + std::to_string(i);
        ini.set(key, openFiles[i]);
    }
}
