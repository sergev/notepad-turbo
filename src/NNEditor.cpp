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

#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TDrawBuffer
#define Uses_TRect
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TGroup
#define Uses_TWindow
#define Uses_TFrame
#include <tvision/tv.h>
#include <tvision/colors.h>

#include "NNEditor.h"
#include "NNDocument.h"
#include "MacroRecorder.h"

#include "Lexilla.h"
#include "LexillaAccess.h"
#include "ILexer.h"
#include "SciLexer.h"

#include "Scintilla.h"
#include "NNCommands.h"

#include <cstring>
#include <algorithm>
#include <climits>


NNEditor::NNEditor(const TRect &bounds,
                   TScrollBar *hScrollBar,
                   TScrollBar *vScrollBar,
                   TIndicator *indicator,
                   TStringView fileName) noexcept
    : TFileEditor(bounds, hScrollBar, vScrollBar, indicator, fileName)
    , document(std::make_unique<NNDocument>(this))
{
    // Zero-initialize style map to default terminal colors
    for (auto &e : styleMap)
        e.attr = TColorAttr{};
}

NNEditor::~NNEditor()
{
    if (lexer)
        lexer->Release();
}

// --- IEditorAPI ---

void NNEditor::styleSetFore(int styleId, int color)
{
    if (styleId < 0 || styleId >= 256) return;
    setFore(styleMap[styleId].attr, TColorDesired(color));
}

void NNEditor::styleSetBack(int styleId, int color)
{
    if (styleId < 0 || styleId >= 256) return;
    setBack(styleMap[styleId].attr, TColorDesired(color));
}

void NNEditor::styleSetBold(int styleId, bool bold)
{
    if (styleId < 0 || styleId >= 256) return;
    styleMap[styleId].bold = bold;
}

void NNEditor::styleSetItalic(int styleId, bool italic)
{
    if (styleId < 0 || styleId >= 256) return;
    styleMap[styleId].italic = italic;
}

void NNEditor::setKeywords(int set, const char *words)
{
    if (lexer && words)
        lexer->WordListSet(set, words);
}

void NNEditor::setProperty(const char *key, const char *value)
{
    if (lexer && key && value)
        lexer->PropertySet(key, value);
}

void NNEditor::setTabWidth(int width)
{
    // TEditor doesn't have a configurable tab width; we store it for future use
    (void)width;
}

void NNEditor::setUseTabs(bool use)
{
    autoIndent = use;
}

void NNEditor::setLexer(const char *languageName)
{
    if (lexer) {
        lexer->Release();
        lexer = nullptr;
    }
    if (languageName && *languageName) {
        // Reset style map to defaults before applying new language styles
        for (auto &e : styleMap)
            e.attr = TColorAttr{};

        lexer = CreateLexer(languageName);
        if (lexer) {
            lexer->PropertySet("fold", "1");
        }
    }
    invalidateStyles();
}

intptr_t NNEditor::send(unsigned int msg, uintptr_t wParam, intptr_t lParam)
{
    // Handle the Scintilla-style messages that LuaExtension emits
    switch (msg) {
        case 2051: // SCI_STYLESETFORE
            styleSetFore((int)wParam, (int)lParam);
            break;
        case 2052: // SCI_STYLESETBACK
            styleSetBack((int)wParam, (int)lParam);
            break;
        case 2053: // SCI_STYLESETBOLD
            styleSetBold((int)wParam, lParam != 0);
            break;
        case 2054: // SCI_STYLESETITALIC
            styleSetItalic((int)wParam, lParam != 0);
            break;
        case 4005: // SCI_SETKEYWORDS
            if (lParam)
                setKeywords((int)wParam, reinterpret_cast<const char*>(lParam));
            break;
        case 4004: // SCI_SETPROPERTY
            if (wParam && lParam)
                setProperty(reinterpret_cast<const char*>(wParam),
                            reinterpret_cast<const char*>(lParam));
            break;
        case 2004: // SCI_SETTABWIDTH
            setTabWidth((int)wParam);
            break;
        case 2124: // SCI_SETUSETABS
            setUseTabs(wParam != 0);
            break;
        case 4006: // SCI_SETLEXERLANGUAGE
            if (lParam)
                setLexer(reinterpret_cast<const char *>(lParam));
            break;
        default:
            break;
    }
    return 0;
}

void NNEditor::invalidateStyles(int fromPos)
{
    lexDirty = true;
    document->onBufferChanged(fromPos);
}

// --- Text access ---

std::string NNEditor::flatText() const
{
    std::string result;
    result.resize(bufLen);
    for (uint i = 0; i < bufLen; ++i) {
        uint p = (i < curPtr) ? i : (i + gapLen);
        result[i] = buffer[p];
    }
    return result;
}

int NNEditor::selectionStart() const
{
    return (int)std::min(selStart, selEnd);
}

int NNEditor::selectionEnd() const
{
    return (int)std::max(selStart, selEnd);
}

void NNEditor::replaceSelection(const std::string &text)
{
    deleteSelect();
    insertText(text.c_str(), (uint)text.size(), False);
}

void NNEditor::replaceAll(const std::string &newContent)
{
    setSelect(0, bufLen, True);
    deleteSelect();
    insertText(newContent.c_str(), (uint)newContent.size(), False);
}

// --- Macro replay ---

void NNEditor::replayMacroStep(const NNMacroStep &step)
{
    switch (step.cmd) {
        case NNMacroCmd::InsertText:
            insertText(step.text.c_str(), (uint)step.text.size(), False);
            break;
        case NNMacroCmd::Delete:
            if (curPtr < bufLen)
                deleteRange(curPtr, nextChar(curPtr), True);
            break;
        case NNMacroCmd::CharLeft:
            if (curPtr != 0) setCurPtr(prevChar(curPtr), 0);
            break;
        case NNMacroCmd::CharRight:
            if (curPtr < bufLen) setCurPtr(nextChar(curPtr), 0);
            break;
        case NNMacroCmd::LineUp:
            setCurPtr(visibleLineMove(curPtr, -1), 0);
            break;
        case NNMacroCmd::LineDown:
            setCurPtr(visibleLineMove(curPtr, 1), 0);
            break;
        case NNMacroCmd::WordLeft:
            setCurPtr(prevWord(curPtr), 0);
            break;
        case NNMacroCmd::WordRight:
            setCurPtr(nextWord(curPtr), 0);
            break;
        case NNMacroCmd::LineStart:
            setCurPtr(lineStart(curPtr), 0);
            break;
        case NNMacroCmd::LineEnd:
            setCurPtr(lineEnd(curPtr), 0);
            break;
        case NNMacroCmd::SelectAll:
            setSelect(0, bufLen, True);
            break;
        case NNMacroCmd::PageUp:
            setCurPtr(visibleLineMove(curPtr, -(size.y - 1)), 0);
            break;
        case NNMacroCmd::PageDown:
            setCurPtr(visibleLineMove(curPtr, size.y - 1), 0);
            break;
        case NNMacroCmd::Cut:   clipCut(); break;
        case NNMacroCmd::Copy:  clipCopy(); break;
        case NNMacroCmd::Paste: clipPaste(); break;
        case NNMacroCmd::DeleteLine: {
            uint s = lineStart(curPtr);
            uint e = nextLine(curPtr);
            deleteRange(s, e, True);
            break;
        }
        default: break;
    }
}

// --- Drawing ---

TColorAttr NNEditor::styleToAttr(uint8_t style) const noexcept
{
    return styleMap[style].attr;
}

bool NNEditor::isFoldHeader(int line) const noexcept
{
    return (document->getFoldLevel(line) & SC_FOLDLEVELHEADERFLAG) != 0;
}

bool NNEditor::isLineHidden(int line) const noexcept
{
    for (int h : collapsedLines) {
        if (line > h && line <= foldEndLine(h))
            return true;
    }
    return false;
}

int NNEditor::visibleRowsBetween(int fromLine, int toLine) const noexcept
{
    int count = 0;
    for (int l = fromLine; l < toLine; ++l)
        if (!isLineHidden(l)) ++count;
    return count;
}

uint NNEditor::skipHiddenForward(uint ptr) noexcept
{
    while (ptr < bufLen &&
           isLineHidden((int)document->LineFromPosition((Sci_Position)ptr)))
        ptr = nextLine(ptr);
    return ptr;
}

uint NNEditor::skipHiddenBackward(uint ptr) noexcept
{
    uint ls = lineStart(ptr);
    while (ls > 0 &&
           isLineHidden((int)document->LineFromPosition((Sci_Position)ls)))
        ls = lineStart(prevChar(ls));
    return ls;
}

uint NNEditor::visibleLineMove(uint ptr, int count) noexcept
{
    uint i = ptr;
    ptr = lineStart(ptr);
    int pos = charPos(ptr, i);
    while (count != 0) {
        if (count < 0) {
            do { ptr = prevLine(ptr); }
            while (ptr > 0 &&
                   isLineHidden((int)document->LineFromPosition((Sci_Position)ptr)));
            ++count;
        } else {
            do { ptr = nextLine(ptr); }
            while (ptr < bufLen &&
                   isLineHidden((int)document->LineFromPosition((Sci_Position)ptr)));
            --count;
        }
    }
    return charPtr(ptr, pos);
}

int NNEditor::foldEndLine(int startLine) const noexcept
{
    int startDepth = document->getFoldLevel(startLine) & SC_FOLDLEVELNUMBERMASK;
    int totalLines = (int)document->LineFromPosition((Sci_Position)bufLen);
    int last = startLine;
    for (int l = startLine + 1; l <= totalLines; ++l) {
        int depth = document->getFoldLevel(l) & SC_FOLDLEVELNUMBERMASK;
        if (depth > startDepth)
            last = l;
        else
            break;
    }
    return last;
}

void NNEditor::toggleFold()
{
    if (!lexer) return;
    int line = (int)document->LineFromPosition((Sci_Position)curPtr);
    if (!isFoldHeader(line)) return;
    if (collapsedLines.count(line))
        collapsedLines.erase(line);
    else
        collapsedLines.insert(line);
    if (isLineHidden((int)document->LineFromPosition((Sci_Position)curPtr)))
        setCurPtr(skipHiddenForward(curPtr), 0);
    drawView();
}

void NNEditor::toggleFoldLine(int line)
{
    if (!lexer || !isFoldHeader(line)) return;
    if (collapsedLines.count(line))
        collapsedLines.erase(line);
    else
        collapsedLines.insert(line);
    if (isLineHidden((int)document->LineFromPosition((Sci_Position)curPtr)))
        setCurPtr(skipHiddenForward(curPtr), 0);
    drawView();
}

int NNEditor::foldMarkerAtRow(int visRow) const noexcept
{
    int line = (int)delta.y;
    int totalLines = (int)document->LineFromPosition((Sci_Position)bufLen);
    int row = 0;
    while (line <= totalLines) {
        if (!isLineHidden(line)) {
            if (row == visRow) {
                if (!isFoldHeader(line)) return 0;
                return collapsedLines.count(line) ? -1 : 1;
            }
            ++row;
        }
        ++line;
    }
    return 0;
}

int NNEditor::lineAtRow(int visRow) const noexcept
{
    int line = (int)delta.y;
    int totalLines = (int)document->LineFromPosition((Sci_Position)bufLen);
    int row = 0;
    while (line <= totalLines) {
        if (!isLineHidden(line)) {
            if (row == visRow) return line;
            ++row;
        }
        ++line;
    }
    return -1;
}

void NNEditor::foldAll(bool collapse)
{
    if (!lexer) return;
    int totalLines = (int)document->LineFromPosition((Sci_Position)bufLen);
    collapsedLines.clear();
    if (collapse) {
        for (int l = 0; l <= totalLines; ++l) {
            if (isFoldHeader(l))
                collapsedLines.insert(l);
        }
    }
    if (isLineHidden((int)document->LineFromPosition((Sci_Position)curPtr)))
        setCurPtr(skipHiddenForward(curPtr), 0);
    drawView();
}

void NNEditor::runLexer()
{
    if (!lexDirty || !lexer)
        return;

    Sci_Position len = (Sci_Position)bufLen;
    if (len == 0) {
        lexDirty = false;
        return;
    }

    document->styleArray.assign(len, 0);
    lexer->Lex(0, len, 0, document.get());
    lexer->Fold(0, len, 0, document.get());
    lexDirty = false;
}

void NNEditor::draw()
{
    // Sync drawPtr with scroll position (same as TEditor::draw)
    if (drawLine != delta.y) {
        drawPtr = lineMove(drawPtr, delta.y - drawLine);
        drawLine = delta.y;
    }

    if (!lexer) {
        // No lexer: fall back to default TEditor drawing
        TEditor::draw();
        return;
    }

    runLexer();

    // Draw each visible line with syntax highlighting.
    // Lines inside a collapsed fold are skipped entirely.
    const int textW = size.x;

    TDrawBuffer textBuf;
    TAttrPair baseColors = getColor(0x0201);
    // Lexilla/Lua supply RGB backgrounds that become uneven greys in TUI; keep normal-text bg.
    const TColorAttr baseLineAttr = baseColors[0];

    // If drawPtr lands inside a collapsed fold (e.g. after scrolling), skip forward.
    uint linePtr = drawPtr;
    while (linePtr < bufLen &&
           isLineHidden((int)document->LineFromPosition((Sci_Position)linePtr)))
        linePtr = nextLine(linePtr);

    int count = size.y;
    int y = 0;

    while (count-- > 0) {
        // --- Text: first pass via formatLine (tab expansion, unicode, selection) ---
        formatLine(textBuf, linePtr, delta.x, textW, baseColors);

        // --- Text: second pass — per-character syntax colors ---
        uint P = linePtr;
        int pos = 0;
        int x = 0;

        while (P < bufLen && x < textW) {
            uint nextP = P;
            int nextPos = pos;
            nextCharAndPos(nextP, nextPos);

            char c = bufChar(P);
            if (c == '\r' || c == '\n')
                break;

            if (nextPos > (int)delta.x) {
                bool inSelection = (selStart <= P && P < selEnd);
                if (!inSelection && P < document->styleArray.size()) {
                    TColorAttr syntaxAttr = styleToAttr(document->styleArray[P]);
                    setBack(syntaxAttr, getBack(baseLineAttr));
                    int charWidth = nextPos - std::max(pos, (int)delta.x);
                    for (int i = x; i < x + charWidth && i < textW; ++i)
                        textBuf.putAttribute(i, syntaxAttr);
                }
                x += nextPos - std::max(pos, (int)delta.x);
            }

            P = nextP;
            pos = nextPos;
        }

        writeBuf(0, y, textW, 1, textBuf);

        // Advance to next visible line, skipping any hidden inside a collapsed fold.
        linePtr = nextLine(linePtr);
        while (linePtr < bufLen &&
               isLineHidden((int)document->LineFromPosition((Sci_Position)linePtr)))
            linePtr = nextLine(linePtr);

        ++y;
    }

    // Compute visual row accounting for hidden lines between viewport top and cursor.
    int visRow = visibleRowsBetween((int)delta.y, (int)curPos.y);
    setCursor(curPos.x - (int)delta.x, visRow);

    // Keep frame fold markers in sync with the current viewport.
    TWindow *win = dynamic_cast<TWindow*>(owner);
    if (win && win->frame)
        win->frame->drawView();
}

void NNEditor::handleEvent(TEvent &event)
{
    // TEditor::handleEvent unconditionally clears all keydown events, so fold
    // shortcuts must be intercepted here before the base class consumes them.
    if (event.what == evKeyDown && event.keyDown.keyCode == kbAltMinus) {
        toggleFold();
        clearEvent(event);
        return;
    }

    // Intercept PageUp/PageDown to move by visible rows, not buffer rows.
    // TEditor would call lineMove(curPtr, ±(size.y-1)) which counts all buffer
    // lines; with hidden folds that traverses far fewer than a page of visible rows.
    if (!collapsedLines.empty() && event.what == evKeyDown) {
        ushort key = event.keyDown.keyCode;
        bool isDown = (key == kbPgDn || key == kbCtrlC);
        bool isUp   = (key == kbPgUp || key == kbCtrlR);
        if (isDown || isUp) {
            uchar selectMode = (event.keyDown.controlKeyState & kbShift) ? smExtend : 0;
            int rows = isDown ? (size.y - 1) : -(size.y - 1);
            setCurPtr(visibleLineMove(curPtr, rows), selectMode);

            // trackCursor(False) uses raw buffer-line arithmetic:
            //   new_delta.y = max(curPos.y - (size.y-1), min(delta.y, curPos.y))
            // When the cursor jumped over hidden lines, curPos.y is much larger
            // than delta.y + (size.y-1), so delta.y leaps forward past the fold.
            // Instead compute delta.y fold-aware: walk back from curPos.y counting
            // size.y-1 visible lines to find where the viewport top should be.
            int curLine = (int)curPos.y;
            int newDelta;
            if (curLine < (int)delta.y) {
                // Cursor moved above old viewport top: place cursor at row 0.
                newDelta = curLine;
            } else {
                int visRow = visibleRowsBetween((int)delta.y, curLine);
                if (visRow < size.y) {
                    // Cursor is still within the viewport: no scroll needed.
                    newDelta = (int)delta.y;
                } else {
                    // Cursor is below the viewport: place cursor at last visible row.
                    // Walk backwards from curLine counting size.y-1 visible lines.
                    int count = 0;
                    int line = curLine - 1;
                    while (line >= 0 && count < size.y - 1) {
                        if (!isLineHidden(line))
                            count++;
                        line--;
                    }
                    newDelta = std::max(0, line + 1);
                }
            }
            scrollTo((int)delta.x, newDelta);

            drawView();
            if (lexer) {
                int visRow = visibleRowsBetween((int)delta.y, (int)curPos.y);
                setCursor(curPos.x - (int)delta.x, visRow);
            }
            clearEvent(event);
            return;
        }
    }

    uint prevPtr = curPtr;
    TFileEditor::handleEvent(event);

    // Snap cursor off hidden lines (arrow keys, PgUp/Dn, mouse clicks).
    if (!collapsedLines.empty()) {
        int curLine = (int)document->LineFromPosition((Sci_Position)curPtr);
        if (isLineHidden(curLine)) {
            uint snapped = (curPtr >= prevPtr)
                ? skipHiddenForward(curPtr)
                : skipHiddenBackward(curPtr);
            setCurPtr(snapped, 0);
        }
        // TEditor::doUpdate() single-line fast-path (ufLine) calls
        // drawLines(curPos.y - delta.y, ...) using a raw buffer-line delta,
        // which puts text at the wrong screen row when hidden lines exist.
        // Force a full redraw so NNEditor::draw() handles row mapping correctly.
        drawView();
    }

    // doUpdate() (called from unlock() inside handleEvent) sets the cursor to
    // curPos.x - delta.x; correct the visual row for hidden lines.
    if (lexer) {
        int visRow = visibleRowsBetween((int)delta.y, (int)curPos.y);
        setCursor(curPos.x - (int)delta.x, visRow);
    }

    // Invalidate syntax highlighting on any text modification
    if (event.what == evBroadcast && modified)
        invalidateStyles();
}
