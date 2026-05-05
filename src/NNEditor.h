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

#pragma once

#define Uses_TFileEditor
#define Uses_TScrollBar
#define Uses_TIndicator
#define Uses_TRect
#include <tvision/tv.h>

#include "IEditorAPI.h"
#include "MacroStep.h"
#include "ILexer.h"

#include <array>
#include <memory>
#include <set>
#include <string>
#include <vector>

class NNDocument;
class MacroRecorder;

// TFileEditor subclass with Lexilla-based syntax highlighting.
// Overrides draw() to apply per-character ANSI color attributes
// based on Lexilla style tokens.
class NNEditor : public TFileEditor, public IEditorAPI {
public:
    NNEditor(const TRect &bounds,
             TScrollBar *hScrollBar,
             TScrollBar *vScrollBar,
             TIndicator *indicator,
             TStringView fileName) noexcept;
    ~NNEditor() override;

    // --- IEditorAPI ---
    void styleSetFore(int styleId, int color) override;
    void styleSetBack(int styleId, int color) override;
    void styleSetBold(int styleId, bool bold) override;
    void styleSetItalic(int styleId, bool italic) override;
    void setKeywords(int set, const char *words) override;
    void setProperty(const char *key, const char *value) override;
    void setTabWidth(int width) override;
    void setUseTabs(bool use) override;
    void setLexer(const char *languageName) override;
    const char *getFileName() const override { return fileName; }
    intptr_t send(unsigned int msg, uintptr_t wParam, intptr_t lParam) override;

    // --- TView overrides ---
    void draw() override;
    void handleEvent(TEvent &event) override;

    // --- Text access (gap-buffer aware) ---
    std::string flatText() const;
    int textLength() const { return (int)bufLen; }
    int cursorPos() const  { return (int)curPtr; }
    int selectionStart() const;
    int selectionEnd() const;
    void replaceSelection(const std::string &text);
    void replaceAll(const std::string &newContent);

    // --- Fold support ---
    void toggleFold();
    void foldAll(bool collapse);
    uint skipHiddenForward(uint ptr) noexcept;

    // --- Macro support ---
    void setRecorder(MacroRecorder *rec) { recorder = rec; }
    void replayMacroStep(const NNMacroStep &step);

    // Called by NNDocument when content changes
    void invalidateStyles(int fromPos = 0);

private:
    struct StyleEntry {
        TColorAttr attr;
        bool bold   = false;
        bool italic = false;
    };

    Scintilla::ILexer5 *lexer = nullptr;
    std::unique_ptr<NNDocument> document;
    MacroRecorder *recorder = nullptr;

    std::array<StyleEntry, 256> styleMap{};
    std::set<int> collapsedLines;

    bool lexDirty = true;

    bool isFoldHeader(int line) const noexcept;
    bool isLineHidden(int line) const noexcept;
    int  foldEndLine(int startLine) const noexcept;
    int  visibleRowsBetween(int fromLine, int toLine) const noexcept;
    uint skipHiddenBackward(uint ptr) noexcept;
    uint visibleLineMove(uint ptr, int count) noexcept;
    void runLexer();
    TColorAttr styleToAttr(uint8_t style) const noexcept;
};
