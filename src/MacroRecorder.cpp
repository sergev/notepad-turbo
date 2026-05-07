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

#include "MacroRecorder.h"

void MacroRecorder::startRecording()
{
    macro = new Macro();
}

Macro *MacroRecorder::stopRecording()
{
    Macro *m = macro;
    macro = nullptr;
    return m;
}

void MacroRecorder::recordStep(NNMacroCmd cmd, const std::string &text)
{
    if (macro) {
        macro->addStep(cmd, text);
    }
}
