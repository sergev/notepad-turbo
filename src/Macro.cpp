/*
 * This file is part of Notepad Turbo.
 * Copyright 2022 Justin Dailey
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "Macro.h"

Macro::Macro() : name("<Current Recorded Macro>") {}

void Macro::addStep(NNMacroCmd cmd, const std::string &text)
{
    // Combine consecutive InsertText steps
    if (cmd == NNMacroCmd::InsertText && !steps.empty() && steps.back().cmd == NNMacroCmd::InsertText) {
        steps.back().text += text;
    } else {
        steps.push_back(NNMacroStep(cmd, text));
    }
}

void Macro::addStep(NNMacroStep step)
{
    steps.push_back(std::move(step));
}
