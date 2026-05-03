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

#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TButton
#define Uses_TCheckBoxes
#define Uses_TLabel
#define Uses_TStaticText
#define Uses_TSItem
#define Uses_TRect
#define Uses_TProgram
#define Uses_TDeskTop
#define Uses_TObject
#include <tvision/tv.h>

#include "NNPreferencesDialog.h"
#include "IniSettings.h"

#include <cstring>
#include <cstdlib>

NNPreferencesDialog::NNPreferencesDialog(IniSettings *settings)
    : settings(settings)
{
}

void NNPreferencesDialog::run()
{
    TDialog *d = new TDialog(TRect(0, 0, 44, 18), "Preferences");
    d->options |= ofCentered;

    // Tab size input
    TInputLine *tabInput = new TInputLine(TRect(20, 2, 25, 3), 4);
    d->insert(tabInput);
    d->insert(new TLabel(TRect(2, 2, 18, 3), "~T~ab width", tabInput));

    // Checkboxes
    d->insert(new TCheckBoxes(TRect(2, 4, 42, 10),
        new TSItem("~U~se tabs (instead of spaces)",
        new TSItem("~W~ord wrap",
        new TSItem("~R~estore previous session",
        new TSItem("~S~how whitespace",
        nullptr))))));

    d->insert(new TButton(TRect(8,  14, 19, 16), "O~K~",    cmOK,     bfDefault));
    d->insert(new TButton(TRect(24, 14, 35, 16), "Cancel",  cmCancel, bfNormal));

    d->selectNext(False);

    // Pack data: tab width string + checkbox bits (ushort)
    struct PrefData {
        char tabWidth[5];
        ushort checkboxes;
    } data;

    snprintf(data.tabWidth, sizeof(data.tabWidth), "%d",
             settings->getInt("Editor.tab_size", 4));

    data.checkboxes = 0;
    if (settings->getBool("Editor.use_tabs",   false)) data.checkboxes |= 0x01;
    if (settings->getBool("Editor.word_wrap",  false)) data.checkboxes |= 0x02;
    if (settings->getBool("Session.restore_previous", true)) data.checkboxes |= 0x04;
    if (settings->getBool("Editor.show_whitespace", false)) data.checkboxes |= 0x08;

    TView *p = TProgram::application->validView(d);
    if (!p) return;

    p->setData(&data);
    ushort result = TProgram::deskTop->execView(p);
    if (result != cmCancel)
        p->getData(&data);
    TObject::destroy(p);

    if (result == cmCancel)
        return;

    settings->setInt("Editor.tab_size",   atoi(data.tabWidth));
    settings->setBool("Editor.use_tabs",  (data.checkboxes & 0x01) != 0);
    settings->setBool("Editor.word_wrap", (data.checkboxes & 0x02) != 0);
    settings->setBool("Session.restore_previous", (data.checkboxes & 0x04) != 0);
    settings->setBool("Editor.show_whitespace",   (data.checkboxes & 0x08) != 0);
}
