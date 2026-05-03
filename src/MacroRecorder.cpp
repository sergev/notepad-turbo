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
#include "NNEditor.h"

void MacroRecorder::startRecording(NNEditor *editor_)
{
    editor = editor_;
    macro = new Macro();
    editor->setRecorder(this);
}

Macro *MacroRecorder::stopRecording()
{
    if (editor) {
        editor->setRecorder(nullptr);
        editor = nullptr;
    }

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
