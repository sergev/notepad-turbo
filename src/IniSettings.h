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

#pragma once

#include <map>
#include <string>
#include <vector>

// Simple INI file settings store. Keys are addressed as "section.key".
// Section and key names must not contain '.' or '=' characters.
class IniSettings {
public:
    explicit IniSettings(std::string path);

    void load();
    void save() const;

    std::string get(const std::string &key, const std::string &def = "") const;
    int getInt(const std::string &key, int def = 0) const;
    bool getBool(const std::string &key, bool def = false) const;

    void set(const std::string &key, const std::string &value);
    void setInt(const std::string &key, int value);
    void setBool(const std::string &key, bool value);

    void remove(const std::string &key);
    void removeSection(const std::string &section);

    // Returns all keys whose names start with prefix
    std::vector<std::string> keysWithPrefix(const std::string &prefix) const;

    static std::string defaultConfigDir(const char *appName);

private:
    std::string filePath;
    std::map<std::string, std::string> data;
};
