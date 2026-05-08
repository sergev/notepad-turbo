/*
 * This file is part of Notepad Turbo.
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once
#include <string>

class NNEncodingDialog {
public:
    explicit NNEncodingDialog(const std::string &currentCharset);
    std::string run(); // Returns selected charset, or "" if cancelled
private:
    std::string currentCharset;
};
