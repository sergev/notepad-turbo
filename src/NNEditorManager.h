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

#include <vector>
#include <string>

class NNEditor;
class NNWindow;
class IniSettings;
class LuaState;

// Tracks open NNWindow instances and provides language detection.
class NNEditorManager {
public:
    NNEditorManager();

    // Called when a new NNWindow is opened so we can track it
    void trackWindow(NNWindow *window);
    void untrackWindow(NNWindow *window);

    const std::vector<NNWindow*> &windows() const { return openWindows; }

    // Apply language detection and Lua-based setup for a given editor
    // (must be called after LuaState is initialized)
    void applyLanguage(NNEditor *editor, const std::string &filePath, LuaState *lua);

private:
    std::vector<NNWindow*> openWindows;

    std::string detectLanguageFromExtension(const std::string &filePath, LuaState *lua);
};
