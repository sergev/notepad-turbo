/*
 * This file is part of Notepad Next.
 * Copyright 2022 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "MacroStep.h"
#include <vector>
#include <string>

class NNEditor;

class Macro
{
public:
    Macro();

    void addStep(NNMacroCmd cmd, const std::string &text = {});
    void addStep(NNMacroStep step);

    int size() const { return (int)steps.size(); }

    void replay(NNEditor *editor, int n = 1) const;
    void replayTillEndOfFile(NNEditor *editor) const;

    const std::string &getName() const { return name; }
    void setName(const std::string &value) { name = value; }

    std::vector<NNMacroStep> &getSteps() { return steps; }
    const std::vector<NNMacroStep> &getSteps() const { return steps; }

private:
    std::vector<NNMacroStep> steps;
    std::string name;
};
