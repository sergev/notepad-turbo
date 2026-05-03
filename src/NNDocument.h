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

#include "ILexer.h"   // Scintilla::IDocument, Sci_Position

#include <vector>
#include <cstdint>

class NNEditor;

// Adapts NNEditor's gap buffer to the Scintilla::IDocument interface
// so that Lexilla lexers can style the text buffer.
class NNDocument : public Scintilla::IDocument {
public:
    explicit NNDocument(NNEditor *editor);
    virtual ~NNDocument() = default;

    // --- IDocument implementation ---
    int SCI_METHOD Version() const noexcept override;
    void SCI_METHOD SetErrorStatus(int status) noexcept override;
    Sci_Position SCI_METHOD Length() const noexcept override;
    void SCI_METHOD GetCharRange(char *buffer, Sci_Position position, Sci_Position lengthRetrieve) const noexcept override;
    char SCI_METHOD StyleAt(Sci_Position position) const noexcept override;
    Sci_Position SCI_METHOD LineFromPosition(Sci_Position position) const noexcept override;
    Sci_Position SCI_METHOD LineStart(Sci_Position line) const noexcept override;
    int SCI_METHOD GetLevel(Sci_Position line) const noexcept override;
    int SCI_METHOD SetLevel(Sci_Position line, int level) noexcept override;
    int SCI_METHOD GetLineState(Sci_Position line) const noexcept override;
    int SCI_METHOD SetLineState(Sci_Position line, int state) noexcept override;
    void SCI_METHOD StartStyling(Sci_Position position) noexcept override;
    bool SCI_METHOD SetStyleFor(Sci_Position length, char style) noexcept override;
    bool SCI_METHOD SetStyles(Sci_Position length, const char *styles) noexcept override;
    void SCI_METHOD DecorationSetCurrentIndicator(int indicator) noexcept override {}
    void SCI_METHOD DecorationFillRange(Sci_Position position, int value, Sci_Position fillLength) noexcept override {}
    void SCI_METHOD ChangeLexerState(Sci_Position start, Sci_Position end) noexcept override;
    int SCI_METHOD CodePage() const noexcept override;
    bool SCI_METHOD IsDBCSLeadByte(char ch) const noexcept override;
    const char * SCI_METHOD BufferPointer() noexcept override;
    int SCI_METHOD GetLineIndentation(Sci_Position line) noexcept override;
    Sci_Position SCI_METHOD LineEnd(Sci_Position line) const noexcept override;
    Sci_Position SCI_METHOD GetRelativePosition(Sci_Position positionStart, Sci_Position characterOffset) const noexcept override;
    int SCI_METHOD GetCharacterAndWidth(Sci_Position position, Sci_Position *pWidth) const noexcept override;

    // Called when the buffer content changes — invalidates flat buffer cache
    void onBufferChanged(Sci_Position firstChanged);

    // The style array: indexed by logical text position
    std::vector<uint8_t> styleArray;

private:
    NNEditor *ed;
    mutable std::vector<char> flatBuf;   // flat (gap-collapsed) buffer cache
    mutable bool flatBufDirty = true;

    std::vector<int> lineState;
    std::vector<int> foldLevel;

    Sci_Position stylingPos = 0;
    Sci_Position lexDirtyFrom = 0;       // first position that needs re-lexing

    void rebuildFlatBuf() const;
    void ensureStyleArray(Sci_Position len);
};
