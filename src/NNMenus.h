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

#define Uses_TMenuBar
#define Uses_TStatusLine
#define Uses_TRect
#include <tvision/tv.h>

// Factory functions called by TProgInit
TMenuBar    *createNNMenuBar(TRect r);
TStatusLine *createNNStatusLine(TRect r);
