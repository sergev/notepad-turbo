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
#include "IniSettings.h"
#include "NNEditor.h"
#include <cstdio>

MacroManager::~MacroManager()
{
    for (Macro *m : macros) delete m;
    if (!isCurrentMacroSaved) delete currentMacro;
}

void MacroManager::startRecording(NNEditor *editor)
{
    if (_isRecording) return;

    _isRecording = true;
    recorder.startRecording(editor);

    if (onRecordingStarted) onRecordingStarted();
}

void MacroManager::stopRecording()
{
    if (!_isRecording) return;

    _isRecording = false;

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

void MacroManager::saveCurrentMacro(const std::string &macroName)
{
    if (!currentMacro) return;

    isCurrentMacroSaved = true;
    currentMacro->setName(macroName);
    macros.push_back(currentMacro);
}

bool MacroManager::hasCurrentUnsavedMacro() const
{
    return currentMacro != nullptr && !isCurrentMacroSaved;
}

void MacroManager::loadSettings(IniSettings &ini)
{
    int count = ini.getInt("Macros.count", 0);
    for (int i = 0; i < count; ++i) {
        std::string prefix = "Macros.macro" + std::to_string(i) + ".";
        std::string name = ini.get(prefix + "name");
        int nsteps = ini.getInt(prefix + "steps", 0);
        if (name.empty()) continue;

        Macro *m = new Macro();
        m->setName(name);
        for (int j = 0; j < nsteps; ++j) {
            std::string sv = ini.get(prefix + "step" + std::to_string(j));
            if (sv.empty()) continue;
            size_t comma = sv.find(',');
            uint32_t cmd = (uint32_t)std::stoul(sv.substr(0, comma));
            std::string text;
            if (comma != std::string::npos) {
                std::string hex = sv.substr(comma + 1);
                for (size_t k = 0; k + 1 < hex.size(); k += 2) {
                    text += (char)std::stoul(hex.substr(k, 2), nullptr, 16);
                }
            }
            m->addStep(NNMacroStep((NNMacroCmd)cmd, text));
        }
        macros.push_back(m);
    }
}

void MacroManager::saveSettings(IniSettings &ini) const
{
    ini.setInt("Macros.count", (int)macros.size());
    for (int i = 0; i < (int)macros.size(); ++i) {
        const Macro *m = macros[i];
        std::string prefix = "Macros.macro" + std::to_string(i) + ".";
        ini.set(prefix + "name", m->getName());
        ini.setInt(prefix + "steps", (int)m->getSteps().size());
        for (int j = 0; j < (int)m->getSteps().size(); ++j) {
            const NNMacroStep &s = m->getSteps()[j];
            std::string sv = std::to_string((uint32_t)s.cmd) + ",";
            char buf[3];
            for (unsigned char c : s.text) {
                snprintf(buf, sizeof(buf), "%02x", c);
                sv += buf;
            }
            ini.set(prefix + "step" + std::to_string(j), sv);
        }
    }
}
