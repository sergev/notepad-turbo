/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TButton
#define Uses_TCheckBoxes
#define Uses_TLabel
#define Uses_THistory
#define Uses_TRect
#include <tvision/tv.h>

// Factory for Find and Replace dialogs (used by TEditor::editorDialog callback).
// These are stateless — created fresh each time the user triggers Find/Replace.
class NNFindReplaceDialog {
public:
    static TDialog *createFind();
    static TDialog *createReplace();
};
