/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "IniSettings.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>

#ifdef _WIN32
#include <shlobj.h>
#endif

IniSettings::IniSettings(std::string path)
    : filePath(std::move(path))
{
}

void IniSettings::load()
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return;

    std::string section;
    std::string line;
    while (std::getline(file, line)) {
        // Strip trailing carriage return
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        // Skip empty lines and comments
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;
        // Section header
        if (line[0] == '[') {
            auto end = line.find(']');
            if (end != std::string::npos)
                section = line.substr(1, end - 1);
            continue;
        }
        // Key=value
        auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        std::string fullKey = section.empty() ? key : section + "." + key;
        data[fullKey] = value;
    }
}

void IniSettings::save() const
{
    // Ensure directory exists
    auto dir = std::filesystem::path(filePath).parent_path();
    if (!dir.empty())
        std::filesystem::create_directories(dir);

    // Group keys by section
    std::map<std::string, std::map<std::string, std::string>> sections;
    for (auto &[fullKey, value] : data) {
        auto dot = fullKey.find('.');
        if (dot == std::string::npos) {
            sections[""][fullKey] = value;
        } else {
            sections[fullKey.substr(0, dot)][fullKey.substr(dot + 1)] = value;
        }
    }

    std::ofstream file(filePath);
    if (!file.is_open())
        return;

    // Write global keys first (no section)
    auto it = sections.find("");
    if (it != sections.end()) {
        for (auto &[k, v] : it->second)
            file << k << "=" << v << "\n";
        file << "\n";
    }

    for (auto &[section, keys] : sections) {
        if (section.empty())
            continue;
        file << "[" << section << "]\n";
        for (auto &[k, v] : keys)
            file << k << "=" << v << "\n";
        file << "\n";
    }
}

std::string IniSettings::get(const std::string &key, const std::string &def) const
{
    auto it = data.find(key);
    return it != data.end() ? it->second : def;
}

int IniSettings::getInt(const std::string &key, int def) const
{
    auto it = data.find(key);
    if (it == data.end())
        return def;
    try { return std::stoi(it->second); } catch (...) { return def; }
}

bool IniSettings::getBool(const std::string &key, bool def) const
{
    auto it = data.find(key);
    if (it == data.end())
        return def;
    const auto &v = it->second;
    return v == "true" || v == "1" || v == "yes";
}

void IniSettings::set(const std::string &key, const std::string &value)
{
    data[key] = value;
}

void IniSettings::setInt(const std::string &key, int value)
{
    data[key] = std::to_string(value);
}

void IniSettings::setBool(const std::string &key, bool value)
{
    data[key] = value ? "true" : "false";
}

void IniSettings::remove(const std::string &key)
{
    data.erase(key);
}

void IniSettings::removeSection(const std::string &section)
{
    std::string prefix = section + ".";
    for (auto it = data.begin(); it != data.end(); ) {
        if (it->first.substr(0, prefix.size()) == prefix)
            it = data.erase(it);
        else
            ++it;
    }
}

std::vector<std::string> IniSettings::keysWithPrefix(const std::string &prefix) const
{
    std::vector<std::string> result;
    for (auto &[k, v] : data)
        if (k.substr(0, prefix.size()) == prefix)
            result.push_back(k);
    return result;
}

std::string IniSettings::defaultConfigDir(const char *appName)
{
#ifdef _WIN32
    char buf[MAX_PATH] = {};
    SHGetFolderPathA(nullptr, CSIDL_APPDATA, nullptr, 0, buf);
    return std::string(buf) + "\\" + appName;
#else
    const char *home = getenv("HOME");
    if (!home) home = "/tmp";
    return std::string(home) + "/.config/" + appName;
#endif
}
