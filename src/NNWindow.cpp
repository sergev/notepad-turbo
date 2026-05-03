/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#define Uses_TEditWindow
#define Uses_TFileEditor
#define Uses_TScrollBar
#define Uses_TIndicator
#define Uses_TFrame
#define Uses_TRect
#define Uses_TEvent
#define Uses_TObject
#define Uses_MsgBox
#include <tvision/tv.h>

#include "NNWindow.h"
#include "NNEditor.h"

#include <cstring>

NNWindow::NNWindow(const TRect &bounds, TStringView fileName, int windowNum) noexcept
    : TWindowInit(&TWindow::initFrame),
      TEditWindow(bounds, fileName, windowNum)
{
    // TEditWindow's constructor just created a plain TFileEditor.
    // Replace it with an NNEditor that adds Lexilla syntax highlighting.

    TScrollBar *h   = editor->hScrollBar;
    TScrollBar *v   = editor->vScrollBar;
    TIndicator *ind = editor->indicator;
    TRect r         = editor->getBounds();
    char fName[MAXPATH];
    strncpy(fName, editor->fileName, MAXPATH - 1);
    fName[MAXPATH - 1] = '\0';

    // Remove and destroy the TFileEditor (leaves scrollbars and indicator intact)
    remove(editor);
    TObject::destroy(editor);

    // Insert the NNEditor in its place
    NNEditor *ne = new NNEditor(r, h, v, ind, fName);
    insert(ne);
    editor = ne;   // TEditWindow::editor is TFileEditor* — NNEditor IS-A TFileEditor
}

NNEditor *NNWindow::nnEditor() const
{
    return static_cast<NNEditor *>(editor);
}

const char *NNWindow::getTitle(short maxSize)
{
    if (editor->isClipboard())
        return TEditWindow::getTitle(maxSize);

    bool dirty = editor->modified;
    const char *base = (*editor->fileName == '\0') ? "Untitled" : editor->fileName;

    titleBuf = dirty ? std::string("*") + base : std::string(base);
    return titleBuf.c_str();
}

void NNWindow::handleEvent(TEvent &event)
{
    TEditWindow::handleEvent(event);

    // Redraw title when modification state changes
    if (event.what == evBroadcast && event.message.command == cmUpdateTitle) {
        if (wasModified != (bool)editor->modified) {
            wasModified = editor->modified;
            if (frame)
                frame->drawView();
        }
    }
}

void NNWindow::close()
{
    // TFileEditor::valid() handles the "save modified?" dialog via editorDialog
    TEditWindow::close();
}
