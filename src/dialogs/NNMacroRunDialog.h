/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <vector>

// Returns the name of the selected macro, or empty string on cancel.
class NNMacroRunDialog {
public:
    explicit NNMacroRunDialog(const std::vector<std::string> &macroNames);
    std::string run();

private:
    const std::vector<std::string> &names;
};
