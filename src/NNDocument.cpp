/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#define Uses_TEditor
#include <tvision/tv.h>

#include "NNDocument.h"
#include "NNEditor.h"

NNDocument::NNDocument(NNEditor *editor)
    : ed(editor)
{
}

void NNDocument::rebuildFlatBuf() const
{
    if (!flatBufDirty)
        return;
    Sci_Position len = ed->bufLen;
    flatBuf.resize(len);
    // Gap buffer: [0..curPtr-1] then gap, then [curPtr+gapLen..bufSize-1]
    if (len == 0) {
        flatBufDirty = false;
        return;
    }
    Sci_Position before = (Sci_Position)ed->curPtr;
    Sci_Position after  = (Sci_Position)ed->bufLen - before;
    if (before > 0)
        memcpy(flatBuf.data(), ed->buffer, before);
    if (after > 0)
        memcpy(flatBuf.data() + before, ed->buffer + ed->curPtr + ed->gapLen, after);
    flatBufDirty = false;
}

void NNDocument::ensureStyleArray(Sci_Position len)
{
    if ((Sci_Position)styleArray.size() < len)
        styleArray.resize(len, 0);
}

void NNDocument::onBufferChanged(Sci_Position firstChanged)
{
    flatBufDirty = true;
    lexDirtyFrom = firstChanged;
    ensureStyleArray(ed->bufLen);
}

// --- IDocument implementation ---

int SCI_METHOD NNDocument::Version() const noexcept
{
    return Scintilla::dvRelease4;
}

void SCI_METHOD NNDocument::SetErrorStatus(int) noexcept {}

Sci_Position SCI_METHOD NNDocument::Length() const noexcept
{
    return (Sci_Position)ed->bufLen;
}

void SCI_METHOD NNDocument::GetCharRange(char *buffer, Sci_Position position, Sci_Position lengthRetrieve) const noexcept
{
    rebuildFlatBuf();
    if (position + lengthRetrieve > (Sci_Position)flatBuf.size())
        lengthRetrieve = (Sci_Position)flatBuf.size() - position;
    if (lengthRetrieve > 0)
        memcpy(buffer, flatBuf.data() + position, lengthRetrieve);
}

char SCI_METHOD NNDocument::StyleAt(Sci_Position position) const noexcept
{
    if (position < 0 || position >= (Sci_Position)styleArray.size())
        return 0;
    return (char)styleArray[position];
}

Sci_Position SCI_METHOD NNDocument::LineFromPosition(Sci_Position position) const noexcept
{
    rebuildFlatBuf();
    Sci_Position line = 0;
    for (Sci_Position i = 0; i < position && i < (Sci_Position)flatBuf.size(); ++i) {
        char c = flatBuf[i];
        if (c == '\n')
            ++line;
    }
    return line;
}

Sci_Position SCI_METHOD NNDocument::LineStart(Sci_Position line) const noexcept
{
    rebuildFlatBuf();
    Sci_Position curLine = 0;
    for (Sci_Position i = 0; i < (Sci_Position)flatBuf.size(); ++i) {
        if (curLine == line)
            return i;
        if (flatBuf[i] == '\n')
            ++curLine;
    }
    return (Sci_Position)flatBuf.size();
}

int SCI_METHOD NNDocument::GetLevel(Sci_Position line) const noexcept
{
    if (line < (int)foldLevel.size())
        return foldLevel[line];
    return 0;
}

int SCI_METHOD NNDocument::SetLevel(Sci_Position line, int level) noexcept
{
    if (line >= (int)foldLevel.size())
        foldLevel.resize(line + 1, 0);
    foldLevel[line] = level;
    return 0;
}

int SCI_METHOD NNDocument::GetLineState(Sci_Position line) const noexcept
{
    if (line < (int)lineState.size())
        return lineState[line];
    return 0;
}

int SCI_METHOD NNDocument::SetLineState(Sci_Position line, int state) noexcept
{
    if (line >= (int)lineState.size())
        lineState.resize(line + 1, 0);
    lineState[line] = state;
    return state;
}

void SCI_METHOD NNDocument::StartStyling(Sci_Position position) noexcept
{
    stylingPos = position;
    ensureStyleArray(ed->bufLen);
}

bool SCI_METHOD NNDocument::SetStyleFor(Sci_Position length, char style) noexcept
{
    ensureStyleArray(stylingPos + length);
    for (Sci_Position i = 0; i < length; ++i)
        styleArray[stylingPos + i] = (uint8_t)style;
    stylingPos += length;
    return true;
}

bool SCI_METHOD NNDocument::SetStyles(Sci_Position length, const char *styles) noexcept
{
    ensureStyleArray(stylingPos + length);
    for (Sci_Position i = 0; i < length; ++i)
        styleArray[stylingPos + i] = (uint8_t)styles[i];
    stylingPos += length;
    return true;
}

void SCI_METHOD NNDocument::ChangeLexerState(Sci_Position start, Sci_Position end) noexcept
{
    if (start < lexDirtyFrom)
        lexDirtyFrom = start;
}

int SCI_METHOD NNDocument::CodePage() const noexcept
{
    return 65001;  // UTF-8
}

bool SCI_METHOD NNDocument::IsDBCSLeadByte(char) const noexcept
{
    return false;
}

const char *SCI_METHOD NNDocument::BufferPointer() noexcept
{
    rebuildFlatBuf();
    return flatBuf.data();
}

int SCI_METHOD NNDocument::GetLineIndentation(Sci_Position line) noexcept
{
    Sci_Position start = LineStart(line);
    rebuildFlatBuf();
    int indent = 0;
    for (Sci_Position i = start; i < (Sci_Position)flatBuf.size(); ++i) {
        char c = flatBuf[i];
        if (c == ' ') indent++;
        else if (c == '\t') indent += 4;
        else break;
    }
    return indent;
}

Sci_Position SCI_METHOD NNDocument::LineEnd(Sci_Position line) const noexcept
{
    rebuildFlatBuf();
    Sci_Position curLine = 0;
    for (Sci_Position i = 0; i < (Sci_Position)flatBuf.size(); ++i) {
        if (curLine == line && (flatBuf[i] == '\n' || flatBuf[i] == '\r'))
            return i;
        if (flatBuf[i] == '\n')
            ++curLine;
    }
    return (Sci_Position)flatBuf.size();
}

Sci_Position SCI_METHOD NNDocument::GetRelativePosition(Sci_Position positionStart, Sci_Position characterOffset) const noexcept
{
    return positionStart + characterOffset;
}

int SCI_METHOD NNDocument::GetCharacterAndWidth(Sci_Position position, Sci_Position *pWidth) const noexcept
{
    rebuildFlatBuf();
    if (pWidth) *pWidth = 1;
    if (position < 0 || position >= (Sci_Position)flatBuf.size())
        return 0;
    return (unsigned char)flatBuf[position];
}
