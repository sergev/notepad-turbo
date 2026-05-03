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
#define Uses_MsgBox
#include <tvision/tv.h>

#include "NNGotoLineDialog.h"

#include <cstdlib>

int NNGotoLineDialog::run()
{
    TDialog *d = new TDialog(TRect(0, 0, 30, 9), "Go to Line");
    d->options |= ofCentered;

    TInputLine *input = new TInputLine(TRect(3, 3, 25, 4), 12);
    d->insert(input);
    d->insert(new TLabel(TRect(2, 2, 16, 3), "~L~ine number", input));

    d->insert(new TButton(TRect(4, 6, 13, 8),  "O~K~",    cmOK,     bfDefault));
    d->insert(new TButton(TRect(17, 6, 27, 8), "Cancel",  cmCancel, bfNormal));

    d->selectNext(False);

    TView *p = TProgram::application->validView(d);
    if (!p) return -1;

    char buf[12] = {};
    p->setData(buf);
    ushort result = TProgram::deskTop->execView(p);
    if (result != cmCancel)
        p->getData(buf);
    TObject::destroy(p);

    if (result == cmCancel)
        return -1;

    int line = atoi(buf);
    return line > 0 ? line : -1;
}
