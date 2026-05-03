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

#define Uses_TEditWindow
#define Uses_TRect
#define Uses_TEvent
#include <tvision/tv.h>

#include <string>

class NNEditor;

// TEditWindow subclass. Replaces editor with NNEditor (Lexilla-aware).
class NNWindow : public TEditWindow {
public:
    NNWindow(const TRect &bounds, TStringView fileName, int windowNum) noexcept;

    void handleEvent(TEvent &event) override;
    const char *getTitle(short maxSize) override;
    void close() override;

    NNEditor *nnEditor() const;

    // Tracks modification state for title asterisk
    bool wasModified = false;

private:
    std::string titleBuf;
};
