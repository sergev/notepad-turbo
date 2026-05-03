/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TButton
#define Uses_TLabel
#define Uses_TRect
#define Uses_TProgram
#define Uses_TDeskTop
#define Uses_TObject
#include <tvision/tv.h>

#include "NNMacroSaveDialog.h"

std::string NNMacroSaveDialog::run()
{
    TDialog *d = new TDialog(TRect(0, 0, 36, 9), "Save Macro");
    d->options |= ofCentered;

    TInputLine *input = new TInputLine(TRect(3, 3, 31, 4), 64);
    d->insert(input);
    d->insert(new TLabel(TRect(2, 2, 14, 3), "~N~ame", input));

    d->insert(new TButton(TRect(4, 6, 14, 8),  "O~K~",    cmOK,     bfDefault));
    d->insert(new TButton(TRect(20, 6, 30, 8), "Cancel",  cmCancel, bfNormal));

    d->selectNext(False);

    TView *p = TProgram::application->validView(d);
    if (!p) return {};

    char buf[65] = {};
    p->setData(buf);
    ushort result = TProgram::deskTop->execView(p);
    if (result != cmCancel)
        p->getData(buf);
    TObject::destroy(p);

    if (result == cmCancel || buf[0] == '\0')
        return {};
    return std::string(buf);
}
