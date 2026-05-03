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
#include <vector>

class IniSettings;

class SessionManager
{
public:
    SessionManager() = default;

    void loadSession(IniSettings &ini, std::vector<std::string> &outFiles);
    void saveSession(IniSettings &ini, const std::vector<std::string> &openFiles);
};
