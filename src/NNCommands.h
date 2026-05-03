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

// Custom command codes for NotepadNext TUI.
// Turbo Vision reserves 0-99; editor commands use 82-84, 500-526.
// We use 200+ for application-level commands.

const int cmNNNew          = 200;
const int cmNNOpen         = 201;
const int cmNNSave         = 202;
const int cmNNSaveAs       = 203;
const int cmNNClose        = 204;
const int cmNNCloseAll     = 205;
const int cmNNReload       = 206;

const int cmNNFind         = 210;
const int cmNNReplace      = 211;
const int cmNNFindNext     = 212;
const int cmNNFindPrev     = 213;
const int cmNNGotoLine     = 214;

const int cmNNLineNumbers  = 220;
const int cmNNWordWrap     = 221;
const int cmNNWhitespace   = 222;

const int cmNNMacroRecord  = 230;
const int cmNNMacroStop    = 231;
const int cmNNMacroRun     = 232;
const int cmNNMacroPause   = 233;
const int cmNNMacroSave    = 234;

const int cmNNPreferences  = 240;
const int cmNNAbout        = 241;
const int cmNNSelectAll    = 242;

const int cmNNSortAsc      = 250;
const int cmNNSortDesc     = 251;
const int cmNNSortRandom   = 252;

// Language selection: cmNNLanguageBase + index into language list
const int cmNNLanguageBase = 300;
