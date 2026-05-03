/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "LuaState.h"
#include "lua.hpp"

#include <fstream>
#include <sstream>
#include <cstdio>
#include <filesystem>

// Global scriptsDir accessible from the static Lua searcher
static std::string g_scriptsDir;

static int luaRequireLanguage(lua_State *L)
{
    const char *module = luaL_checkstring(L, 1);
    lua_pop(L, 1);

    std::string path = g_scriptsDir + "/languages/" + module + ".lua";

    std::ifstream f(path);
    if (!f.is_open()) {
        // Also try root of scriptsDir (e.g., init.lua)
        path = g_scriptsDir + "/" + module + ".lua";
        f.open(path);
    }

    if (!f.is_open()) {
        lua_pushfstring(L, "\n\tno file '%s'", path.c_str());
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(f)),
                      std::istreambuf_iterator<char>());
    f.close();

    if (luaL_loadstring(L, code.c_str()) != LUA_OK) {
        // Error string is on stack — return it for Lua's require error message
    }
    return 1;
}

LuaState::LuaState()
{
    L = luaL_newstate();
    luaL_openlibs(L);

    // Replace filesystem searchers with our resource searcher
    // Remove searchers 2, 3, 4 (file-based) and insert ours at index 2
    for (int i = 0; i < 3; ++i) {
        lua_getglobal(L, "table");
        lua_getfield(L, -1, "remove");
        lua_remove(L, -2);
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "searchers");
        lua_remove(L, -2);
        lua_pushnumber(L, 2);   // remove index 2 each time (list shifts down)
        lua_call(L, 2, 0);
    }

    // Insert our searcher at index 2
    lua_getglobal(L, "table");
    lua_getfield(L, -1, "insert");
    lua_remove(L, -2);
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "searchers");
    lua_remove(L, -2);
    lua_pushnumber(L, 2);
    lua_pushcfunction(L, luaRequireLanguage);
    lua_call(L, 3, 0);
}

LuaState::~LuaState()
{
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void LuaState::setScriptsDir(const std::string &dir)
{
    scriptsDir = dir;
    g_scriptsDir = dir;
}

void LuaState::executeInitScript()
{
    std::string initPath = scriptsDir + "/init.lua";
    if (std::filesystem::exists(initPath))
        executeFile(initPath);
    else
        fprintf(stderr, "LuaState: init.lua not found at %s\n", initPath.c_str());
}

void LuaState::setVariable(const char *name, const char *value)
{
    lua_pushstring(L, value);
    lua_setglobal(L, name);
}

void LuaState::setVariable(const char *name, bool value)
{
    lua_pushboolean(L, value ? 1 : 0);
    lua_setglobal(L, name);
}

void LuaState::executeString(const char *statement, bool clear)
{
    const int stacktop = lua_gettop(L);

    int status = luaL_loadstring(L, statement);
    if (status == LUA_OK)
        status = lua_pcall(L, 0, LUA_MULTRET, 0);

    if (status != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    }

    if (clear)
        lua_settop(L, stacktop);
}

void LuaState::executeFile(const std::string &path)
{
    std::ifstream f(path);
    if (!f.is_open()) {
        fprintf(stderr, "LuaState: cannot open file: %s\n", path.c_str());
        return;
    }

    std::string code((std::istreambuf_iterator<char>(f)),
                      std::istreambuf_iterator<char>());
    f.close();

    executeString(code.c_str());
}

void LuaState::clearStack()
{
    lua_settop(L, 0);
}

void LuaState::raiseError(const char *errorMessage)
{
    luaL_where(L, 1);
    if (errorMessage) {
        lua_pushstring(L, errorMessage);
    } else {
        lua_insert(L, -2);
    }
    lua_concat(L, 2);
    lua_error(L);
}
