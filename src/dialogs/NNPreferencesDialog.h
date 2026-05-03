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

class IniSettings;

class NNPreferencesDialog {
public:
    explicit NNPreferencesDialog(IniSettings *settings);
    void run();

private:
    IniSettings *settings;
};
