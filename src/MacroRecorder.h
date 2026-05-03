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

#include "Macro.h"

class NNEditor;

class MacroRecorder
{
public:
    MacroRecorder() = default;

    void startRecording(NNEditor *editor);
    Macro *stopRecording();
    void recordStep(NNMacroCmd cmd, const std::string &text = {});

private:
    NNEditor *editor = nullptr;
    Macro *macro = nullptr;
};
