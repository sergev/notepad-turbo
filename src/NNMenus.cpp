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

#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TKeys
#define Uses_TRect
#include <tvision/tv.h>

#include "NNMenus.h"
#include "NNCommands.h"

TMenuBar *createNNMenuBar(TRect r)
{
    r.b.y = r.a.y + 1;

    TSubMenu &fileMenu = *new TSubMenu("~F~ile", kbAltF) +
        *new TMenuItem("~N~ew",           cmNNNew,    kbCtrlN,  hcNoContext, "Ctrl-N") +
        *new TMenuItem("~O~pen...",       cmNNOpen,   kbF3,     hcNoContext, "F3")    +
        newLine() +
        *new TMenuItem("~S~ave",          cmNNSave,   kbF2,     hcNoContext, "F2")    +
        *new TMenuItem("Save ~a~s...",    cmNNSaveAs, kbNoKey)  +
        newLine() +
        *new TMenuItem("~C~lose",         cmNNClose,  kbCtrlW,  hcNoContext, "Ctrl-W") +
        *new TMenuItem("Close ~A~ll",     cmNNCloseAll, kbNoKey) +
        newLine() +
        *new TMenuItem("E~x~it",          cmQuit,     kbCtrlQ,  hcNoContext, "Ctrl-Q");

    TSubMenu &editMenu = *new TSubMenu("~E~dit", kbAltE) +
        *new TMenuItem("~U~ndo",          cmUndo,     kbCtrlZ,  hcNoContext, "Ctrl-Z") +
        newLine() +
        *new TMenuItem("Cu~t~",           cmCut,      kbCtrlX,  hcNoContext, "Ctrl-X") +
        *new TMenuItem("~C~opy",          cmCopy,     kbCtrlC,  hcNoContext, "Ctrl-C") +
        *new TMenuItem("~P~aste",         cmPaste,    kbCtrlV,  hcNoContext, "Ctrl-V") +
        *new TMenuItem("~D~elete",        cmClear,    kbDel,    hcNoContext, "Del")    +
        newLine() +
        *new TMenuItem("Select ~A~ll",    cmNNSelectAll, kbCtrlA, hcNoContext, "Ctrl-A");

    TSubMenu &searchMenu = *new TSubMenu("~S~earch", kbAltS) +
        *new TMenuItem("~F~ind...",       cmNNFind,      kbCtrlF, hcNoContext, "Ctrl-F") +
        *new TMenuItem("~R~eplace...",    cmNNReplace,   kbCtrlH, hcNoContext, "Ctrl-H") +
        *new TMenuItem("Find ~N~ext",     cmNNFindNext,  kbF4,    hcNoContext, "F4")     +
        *new TMenuItem("Find ~P~revious", cmNNFindPrev,  kbShiftF4, hcNoContext, "Shift-F4") +
        newLine() +
        *new TMenuItem("~G~oto Line...",  cmNNGotoLine,  kbCtrlG, hcNoContext, "Ctrl-G");

    TSubMenu &viewMenu = *new TSubMenu("~V~iew", kbAltV) +
        *new TMenuItem("~W~ord Wrap",     cmNNWordWrap,   kbNoKey) +
        *new TMenuItem("~W~hitespace",    cmNNWhitespace, kbNoKey) +
        newLine() +
        *new TMenuItem("Toggle ~F~old",   cmNNFoldToggle, kbAltMinus, hcNoContext, "Alt--") +
        *new TMenuItem("Fold ~A~ll",      cmNNFoldAll,    kbNoKey) +
        *new TMenuItem("~U~nfold All",    cmNNUnfoldAll,  kbNoKey);

    TSubMenu &toolsMenu = *new TSubMenu("~T~ools", kbAltT) +
        *new TMenuItem("~P~references...", cmNNPreferences, kbNoKey) +
        *new TMenuItem("~E~ncoding...",    cmNNEncoding,    kbNoKey) +
        newLine() +
        *new TMenuItem("~R~ecord Macro",   cmNNMacroRecord, kbNoKey) +
        *new TMenuItem("~S~top Recording", cmNNMacroStop,   kbNoKey) +
        *new TMenuItem("R~u~n Macro",      cmNNMacroRun,    kbNoKey) +
        *new TMenuItem("Sa~v~e Macro",     cmNNMacroSave,   kbNoKey);

    TSubMenu &windowMenu = *new TSubMenu("~W~indow", kbAltW) +
        *new TMenuItem("~T~ile",      cmTile,   kbNoKey) +
        *new TMenuItem("~C~ascade",   cmCascade, kbNoKey) +
        *new TMenuItem("~N~ext",      cmNext,   kbF6,     hcNoContext, "F6")       +
        *new TMenuItem("~P~revious",  cmPrev,   kbShiftF6, hcNoContext, "Shift-F6");

    TSubMenu &helpMenu = *new TSubMenu("~H~elp", kbAltH) +
        *new TMenuItem("~A~bout...",  cmNNAbout, kbNoKey);

    return new TMenuBar(r,
        fileMenu + editMenu + searchMenu + viewMenu + toolsMenu + windowMenu + helpMenu);
}

TStatusLine *createNNStatusLine(TRect r)
{
    r.a.y = r.b.y - 1;
    return new TStatusLine(r,
        *new TStatusDef(0, 0xFFFF) +
            *new TStatusItem(nullptr,        kbAltX,     cmQuit)     +
            *new TStatusItem("~F2~ Save",    kbF2,       cmNNSave)   +
            *new TStatusItem("~F3~ Open",    kbF3,       cmNNOpen)   +
            *new TStatusItem("~F4~ Find Next",kbF4,      cmNNFindNext) +
            *new TStatusItem("~Ctrl-W~ Close", kbCtrlW,  cmNNClose)  +
            *new TStatusItem("~F5~ Zoom",    kbF5,       cmZoom)     +
            *new TStatusItem("~F6~ Next",    kbF6,       cmNext)     +
            *new TStatusItem("~F10~ Menu",   kbF10,      cmMenu)
    );
}
