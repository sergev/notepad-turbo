/*
 * This file is part of Notepad Turbo.
 * Copyright 2024 Justin Dailey
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "MacroManager.h"
#include "NNEditor.h"

void MacroManager::startRecording(NNEditor *editor)
{
    if (_isRecording) return;

    _isRecording = true;
    activeEditor = editor;
    recorder.startRecording();
    editor->setRecorder(&recorder);

    if (onRecordingStarted) onRecordingStarted();
}

void MacroManager::stopRecording()
{
    if (!_isRecording) return;

    _isRecording = false;

    if (activeEditor) {
        activeEditor->setRecorder(nullptr);
        activeEditor = nullptr;
    }

    Macro *m = recorder.stopRecording();

    if (m->size() == 0) {
        delete m;
    } else {
        if (!isCurrentMacroSaved) {
            delete currentMacro;
        }
        isCurrentMacroSaved = false;
        currentMacro = m;
    }

    if (onRecordingStopped) onRecordingStopped();
}

void MacroManager::replayCurrentMacro(NNEditor *editor)
{
    if (currentMacro) currentMacro->replay(editor);
}
