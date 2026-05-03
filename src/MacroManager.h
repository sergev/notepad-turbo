/*
 * This file is part of Notepad Turbo.
 * Copyright 2024 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "MacroRecorder.h"
#include <vector>
#include <string>
#include <functional>

class IniSettings;
class NNEditor;

class MacroManager
{
public:
    MacroManager() = default;
    ~MacroManager();

    bool isRecording() const { return _isRecording; }
    std::vector<Macro *> &availableMacros() { return macros; }

    void replayCurrentMacro(NNEditor *editor);
    void saveCurrentMacro(const std::string &macroName);
    bool hasCurrentUnsavedMacro() const;
    Macro *getCurrentMacro() const { return currentMacro; }

    void startRecording(NNEditor *editor);
    void stopRecording();

    void loadSettings(IniSettings &ini);
    void saveSettings(IniSettings &ini) const;

    std::function<void()> onRecordingStarted;
    std::function<void()> onRecordingStopped;

private:
    MacroRecorder recorder;
    Macro *currentMacro = nullptr;
    std::vector<Macro *> macros;
    bool _isRecording = false;
    bool isCurrentMacroSaved = false;
};
