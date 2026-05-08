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

#define Uses_TEditWindow
#define Uses_TFrame
#define Uses_TRect
#define Uses_TEvent
#include <tvision/tv.h>

#include <functional>
#include <string>

class NNEditor;

// TEditWindow subclass. Replaces editor with NNEditor (Lexilla-aware).
class NNWindow : public TEditWindow {
public:
    NNWindow(const TRect &bounds, TStringView fileName, int windowNum) noexcept;

    void handleEvent(TEvent &event) override;
    const char *getTitle(short maxSize) override;
    void close() override;
    void shutDown() override;

    NNEditor *nnEditor() const;

    static TFrame *initFrame(TRect r);

    std::function<void()> onShutDown;
    bool wasModified = false;

private:
    std::string titleBuf;
};
