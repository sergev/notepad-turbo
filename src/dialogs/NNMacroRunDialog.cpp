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
#define Uses_TListBox
#define Uses_TButton
#define Uses_TLabel
#define Uses_TScrollBar
#define Uses_TRect
#define Uses_TProgram
#define Uses_TDeskTop
#define Uses_TObject
#define Uses_TCollection
#define Uses_TStringCollection
#define Uses_MsgBox
#include <tvision/tv.h>

#include "NNMacroRunDialog.h"

NNMacroRunDialog::NNMacroRunDialog(const std::vector<std::string> &macroNames)
    : names(macroNames)
{
}

std::string NNMacroRunDialog::run()
{
    if (names.empty()) {
        messageBox("No macros defined.", mfInformation | mfOKButton);
        return {};
    }

    TDialog *d = new TDialog(TRect(0, 0, 40, 18), "Run Macro");
    d->options |= ofCentered;

    TScrollBar *sb = new TScrollBar(TRect(37, 3, 38, 15));
    d->insert(sb);

    TListBox *lb = new TListBox(TRect(2, 3, 37, 15), 1, sb);
    d->insert(lb);
    d->insert(new TLabel(TRect(2, 2, 16, 3), "~M~acros", lb));

    d->insert(new TButton(TRect(6,  15, 17, 17), "~R~un",    cmOK,     bfDefault));
    d->insert(new TButton(TRect(22, 15, 33, 17), "Cancel",   cmCancel, bfNormal));

    TView *p = TProgram::application->validView(d);
    if (!p) return {};

    // Populate list
    TStringCollection *col = new TStringCollection(names.size() + 1, 4);
    for (const auto &n : names)
        col->insert(newStr(n.c_str()));
    lb->newList(col);

    ushort sel = 0;
    p->setData(&sel);
    ushort result = TProgram::deskTop->execView(p);
    if (result != cmCancel)
        p->getData(&sel);
    TObject::destroy(p);

    if (result == cmCancel || sel >= names.size())
        return {};
    return names[sel];
}
