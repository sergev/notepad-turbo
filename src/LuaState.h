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
#include "lua.hpp"

class LuaState {
public:
    LuaState();
    ~LuaState();

    void setScriptsDir(const std::string &dir);
    void executeInitScript();

    void setVariable(const char *name, const char *value);
    void setVariable(const char *name, bool value);

    void executeString(const char *statement, bool clear = true);
    void executeFile(const std::string &path);
    void clearStack();

    void raiseError(const char *errorMessage = nullptr);

    lua_State *L = nullptr;

private:
    std::string scriptsDir;
};
