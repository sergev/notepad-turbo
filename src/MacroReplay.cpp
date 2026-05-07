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
#include "NNEditor.h"

void Macro::replay(NNEditor *editor, int n) const
{
    while (n > 0) {
        for (const NNMacroStep &step : steps) {
            editor->replayMacroStep(step);
        }
        --n;
    }
}

void Macro::replayTillEndOfFile(NNEditor *editor) const
{
    do {
        int lengthBefore = editor->textLength();
        int posBefore = editor->cursorPos();

        replay(editor);

        int lengthAfter = editor->textLength();
        int posAfter = editor->cursorPos();

        if (lengthAfter < lengthBefore) continue;
        if (lengthAfter > lengthBefore) {
            int deltaLen = lengthAfter - lengthBefore;
            int deltaPos = posAfter - posBefore;
            if (deltaPos > deltaLen) continue;
        } else {
            if (posAfter != posBefore) continue;
        }

        break;
    } while (true);
}
