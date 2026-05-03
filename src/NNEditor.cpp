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
#include <tvision/tv.h>
#include <tvision/colors.h>

#include "NNEditor.h"
#include "NNDocument.h"
#include "MacroRecorder.h"

#include "Lexilla.h"
#include "LexillaAccess.h"
#include "ILexer.h"
#include "SciLexer.h"

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
            lexer->PropertySet("fold", "0");
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
            setCurPtr(lineMove(curPtr, -1), 0);
            break;
        case NNMacroCmd::LineDown:
            setCurPtr(lineMove(curPtr, 1), 0);
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
            setCurPtr(lineMove(curPtr, -(size.y - 1)), 0);
            break;
        case NNMacroCmd::PageDown:
            setCurPtr(lineMove(curPtr, size.y - 1), 0);
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

    // Draw each visible line with syntax highlighting
    TDrawBuffer b;
    TAttrPair baseColors = getColor(0x0201);
    // Lexilla/Lua supply RGB backgrounds that become uneven greys in TUI; keep normal-text bg.
    const TColorAttr baseLineAttr = baseColors[0];
    uint linePtr = drawPtr;
    int count = size.y;
    int y = 0;

    while (count-- > 0) {
        // First pass: use TEditor's formatLine for text layout (tab expansion, unicode, etc.)
        formatLine(b, linePtr, delta.x, size.x, baseColors);

        // Second pass: apply per-character syntax colors
        // Walk the same way formatLine does, computing screen x positions
        uint P = linePtr;
        int pos = 0;
        int x = 0;

        while (P < bufLen && x < size.x) {
            uint nextP = P;
            int nextPos = pos;
            nextCharAndPos(nextP, nextPos);

            char c = bufChar(P);
            if (c == '\r' || c == '\n')
                break;

            if (nextPos > (int)delta.x) {
                // Don't override the selection highlight
                bool inSelection = (selStart <= P && P < selEnd);
                if (!inSelection && P < document->styleArray.size()) {
                    TColorAttr syntaxAttr = styleToAttr(document->styleArray[P]);
                    setBack(syntaxAttr, getBack(baseLineAttr));
                    int charWidth = nextPos - std::max(pos, (int)delta.x);
                    for (int i = x; i < x + charWidth && i < size.x; ++i)
                        b.putAttribute(i, syntaxAttr);
                }
                x += nextPos - std::max(pos, (int)delta.x);
            }

            P = nextP;
            pos = nextPos;
        }

        writeBuf(0, y, size.x, 1, b);
        linePtr = nextLine(linePtr);
        ++y;
    }
}

void NNEditor::handleEvent(TEvent &event)
{
    TFileEditor::handleEvent(event);
    // Invalidate syntax highlighting on any text modification
    if (event.what == evBroadcast && modified)
        invalidateStyles();
}
