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
#define Uses_TCheckBoxes
#define Uses_TLabel
#define Uses_THistory
#define Uses_TSItem
#define Uses_TRect
#include <tvision/tv.h>

#include "NNFindReplaceDialog.h"

TDialog *NNFindReplaceDialog::createFind()
{
    TDialog *d = new TDialog(TRect(0, 0, 40, 13), "Find");
    d->options |= ofCentered;

    TInputLine *findInput = new TInputLine(TRect(3, 3, 34, 4), 256);
    d->insert(findInput);
    d->insert(new TLabel(TRect(2, 2, 16, 3), "~T~ext to find", findInput));
    d->insert(new THistory(TRect(34, 3, 37, 4), findInput, 10));

    d->insert(new TCheckBoxes(TRect(3, 5, 37, 8),
        new TSItem("~C~ase sensitive",
        new TSItem("~W~hole words only",
        new TSItem("~R~egex", nullptr)))));

    d->insert(new TButton(TRect(5, 10, 16, 12),  "~F~ind Next", cmOK,     bfDefault));
    d->insert(new TButton(TRect(18, 10, 28, 12), "Cancel",      cmCancel, bfNormal));

    d->selectNext(False);
    return d;
}

TDialog *NNFindReplaceDialog::createReplace()
{
    TDialog *d = new TDialog(TRect(0, 0, 42, 17), "Replace");
    d->options |= ofCentered;

    TInputLine *findInput = new TInputLine(TRect(3, 3, 36, 4), 256);
    d->insert(findInput);
    d->insert(new TLabel(TRect(2, 2, 16, 3), "~T~ext to find", findInput));
    d->insert(new THistory(TRect(36, 3, 39, 4), findInput, 10));

    TInputLine *replInput = new TInputLine(TRect(3, 6, 36, 7), 256);
    d->insert(replInput);
    d->insert(new TLabel(TRect(2, 5, 14, 6), "~R~eplacement", replInput));
    d->insert(new THistory(TRect(36, 6, 39, 7), replInput, 11));

    d->insert(new TCheckBoxes(TRect(3, 8, 39, 12),
        new TSItem("~C~ase sensitive",
        new TSItem("~W~hole words only",
        new TSItem("~P~rompt on replace",
        new TSItem("Replace ~A~ll", nullptr))))));

    d->insert(new TButton(TRect(3,  14, 14, 16), "~F~ind Next",   cmOK,     bfDefault));
    d->insert(new TButton(TRect(16, 14, 27, 16), "~R~eplace",     cmYes,    bfNormal));
    d->insert(new TButton(TRect(29, 14, 40, 16), "Cancel",        cmCancel, bfNormal));

    d->selectNext(False);
    return d;
}
