/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TRect
#define Uses_TKeys
#include <tvision/tv.h>

#include "NNStatusLine.h"
#include "NNCommands.h"

TStatusLine *createNNStatusLine(const TRect &r)
{
    return new TStatusLine(r,
        *new TStatusDef(0, 0xFFFF) +
            *new TStatusItem("~F2~ Save",          kbF2,    cmNNSave) +
            *new TStatusItem("~F3~ Open",          kbF3,    cmNNOpen) +
            *new TStatusItem("~F4~ Find",          kbF4,    cmNNFind) +
            *new TStatusItem("~F5~ Goto",          kbF5,    cmNNGotoLine) +
            *new TStatusItem("~F6~ Next",          kbF6,    cmNext) +
            *new TStatusItem("~F10~ Menu",         kbF10,   cmMenu) +
            *new TStatusItem("~^W~ Close",         kbCtrlW, cmNNClose) +
            *new TStatusItem(0,                    kbF1,    cmHelp)
    );
}
