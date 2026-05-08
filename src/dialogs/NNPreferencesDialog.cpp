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
#define Uses_TRadioButtons
#define Uses_TLabel
#define Uses_TStaticText
#define Uses_TSItem
#define Uses_TRect
#define Uses_TProgram
#define Uses_TDeskTop
#define Uses_TObject
#include <tvision/tv.h>

#include "NNPreferencesDialog.h"
#include "FileEncoding.h"
#include "IniSettings.h"

#include <cstring>
#include <cstdlib>

namespace {

ushort policyToIndex(FileEncoding::SavePolicy policy)
{
    switch (policy) {
        case FileEncoding::SavePolicy::Fail:
            return 0;
        case FileEncoding::SavePolicy::Replace:
            return 1;
        case FileEncoding::SavePolicy::Utf8:
            return 2;
    }
    return 1;
}

FileEncoding::SavePolicy indexToPolicy(ushort index)
{
    switch (index) {
        case 0:
            return FileEncoding::SavePolicy::Fail;
        case 2:
            return FileEncoding::SavePolicy::Utf8;
        case 1:
        default:
            return FileEncoding::SavePolicy::Replace;
    }
}

} // namespace

NNPreferencesDialog::NNPreferencesDialog(IniSettings *settings)
    : settings(settings)
{
}

void NNPreferencesDialog::run()
{
    TDialog *d = new TDialog(TRect(0, 0, 54, 22), "Preferences");
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

    d->insert(new TStaticText(TRect(2, 10, 50, 11), "When original encoding cannot represent text:"));
    d->insert(new TRadioButtons(TRect(4, 11, 50, 15),
        new TSItem("~F~ail save",
        new TSItem("~R~eplace characters",
        new TSItem("Save as ~U~TF-8", nullptr)))));

    d->insert(new TButton(TRect(12, 18, 23, 20), "O~K~",    cmOK,     bfDefault));
    d->insert(new TButton(TRect(30, 18, 41, 20), "Cancel",  cmCancel, bfNormal));

    d->selectNext(False);

    // Pack data: tab width string + checkbox bits (ushort)
    struct PrefData {
        char tabWidth[5];
        ushort checkboxes;
        ushort encodingSavePolicy;
    } data;

    snprintf(data.tabWidth, sizeof(data.tabWidth), "%d",
             settings->getInt("Editor.tab_size", 4));

    data.checkboxes = 0;
    if (settings->getBool("Editor.use_tabs",   false)) data.checkboxes |= 0x01;
    if (settings->getBool("Editor.word_wrap",  false)) data.checkboxes |= 0x02;
    if (settings->getBool("Session.restore_previous", true)) data.checkboxes |= 0x04;
    if (settings->getBool("Editor.show_whitespace", false)) data.checkboxes |= 0x08;
    data.encodingSavePolicy = policyToIndex(FileEncoding::parseSavePolicy(
        settings->get("Editor.encoding_save_policy", FileEncoding::DefaultSavePolicyName)));

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
    settings->set("Editor.encoding_save_policy",
                  FileEncoding::savePolicyName(indexToPolicy(data.encodingSavePolicy)));
}
