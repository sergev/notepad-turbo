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

#include "IEditorAPI.h"
#include <vector>

struct lua_State;

class LuaExtension final {
private:
    LuaExtension();
    LuaExtension(const LuaExtension &);
    void operator=(const LuaExtension &);

public:
    static LuaExtension &Instance();

    ~LuaExtension();

    bool Initialise(lua_State *L, IEditorAPI *editor_);
    void setEditor(IEditorAPI *editor_);
    bool Finalise();

    bool RunString(const char *s);
    bool OnExecute(const char *s);
    void CallShortcut(int id);
};
