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

#include <cstdint>

// Minimal editor API used by LuaExtension to configure syntax highlighting
// and indentation settings. Implemented by NNEditor.
struct IEditorAPI {
    virtual ~IEditorAPI() = default;

    virtual void styleSetFore(int styleId, int color) = 0;
    virtual void styleSetBack(int styleId, int color) = 0;
    virtual void styleSetBold(int styleId, bool bold) = 0;
    virtual void styleSetItalic(int styleId, bool italic) = 0;
    virtual void setKeywords(int set, const char *words) = 0;
    virtual void setProperty(const char *key, const char *value) = 0;
    virtual void setTabWidth(int width) = 0;
    virtual void setUseTabs(bool use) = 0;
    virtual void setLexer(const char *languageName) = 0;
    virtual const char *getFileName() const = 0;

    // Generic fallback for IFace messages not covered above
    virtual intptr_t send(unsigned int msg, uintptr_t wParam, intptr_t lParam) = 0;
};
