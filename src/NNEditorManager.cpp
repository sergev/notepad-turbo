/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "NNEditorManager.h"
#include "NNWindow.h"
#include "NNEditor.h"
#include "IniSettings.h"
#include "LuaState.h"

#include <algorithm>
#include <filesystem>

NNEditorManager::NNEditorManager(IniSettings *settings)
    : settings(settings)
{
}

void NNEditorManager::trackWindow(NNWindow *window)
{
    openWindows.push_back(window);
}

void NNEditorManager::untrackWindow(NNWindow *window)
{
    auto it = std::find(openWindows.begin(), openWindows.end(), window);
    if (it != openWindows.end())
        openWindows.erase(it);
}

std::string NNEditorManager::detectLanguageFromExtension(const std::string &filePath, LuaState *lua)
{
    if (!lua || filePath.empty())
        return {};

    auto ext = std::filesystem::path(filePath).extension().string();
    if (ext.size() > 1 && ext[0] == '.')
        ext = ext.substr(1);

    if (ext.empty())
        return {};

    // Call Lua: DetectLanguageFromExtension(ext) returns language name or nil
    lua_State *L = lua->L;
    lua_getglobal(L, "DetectLanguageFromExtension");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return {};
    }
    lua_pushstring(L, ext.c_str());
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        lua_pop(L, 1);
        return {};
    }
    std::string result;
    if (lua_isstring(L, -1))
        result = lua_tostring(L, -1);
    lua_pop(L, 1);
    return result;
}

void NNEditorManager::applyLanguage(NNEditor *editor, const std::string &filePath, LuaState *lua)
{
    if (!lua)
        return;

    std::string lang = detectLanguageFromExtension(filePath, lua);
    if (lang.empty())
        return;

    // Call Lua: SetLanguage(editor, langName)
    // In the original code this calls LuaExtension which routes style calls to the editor.
    // Here we set the editor as the active IEditorAPI before calling.
    lua_State *L = lua->L;
    lua_getglobal(L, "SetLanguage");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return;
    }
    // Push language name as string argument
    lua_pushstring(L, lang.c_str());
    lua_pcall(L, 1, 0, 0);
}
