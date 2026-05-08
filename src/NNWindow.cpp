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

#define Uses_TEditWindow
#define Uses_TFileEditor
#define Uses_TScrollBar
#define Uses_TIndicator
#define Uses_TFrame
#define Uses_TDrawBuffer
#define Uses_TRect
#define Uses_TEvent
#define Uses_TObject
#define Uses_MsgBox
#include <tvision/tv.h>

#include "NNWindow.h"
#include "NNEditor.h"

#include <cstring>

// NNFrame: TFrame subclass that overlays '+'/'-' fold markers on the left border.
class NNFrame : public TFrame {
public:
    NNFrame(const TRect& bounds) noexcept : TFrame(bounds) {}
    void setEditor(NNEditor* ed) noexcept { editor = ed; }
    void draw() override;
    void handleEvent(TEvent& event) override;
private:
    NNEditor* editor = nullptr;
};

void NNFrame::draw()
{
    TFrame::draw();
    if (!editor) return;

    TAttrPair cFrame;
    if ((state & sfDragging) != 0)
        cFrame = getColor(0x0505);
    else if ((state & sfActive) == 0)
        cFrame = getColor(0x0101);
    else
        cFrame = getColor(0x0503);

    for (int y = 1; y <= size.y - 2; ++y) {
        int marker = editor->foldMarkerAtRow(y - 1);
        if (marker == 0) continue;
        const char *str = (marker > 0) ? "⊟" : "⊞";
        TDrawBuffer b;
        b.moveStr(0, str, cFrame[0]);
        writeLine(0, y, 1, 1, b);
    }
}

void NNFrame::handleEvent(TEvent& event)
{
    if (editor && event.what == evMouseDown) {
        TPoint mp = makeLocal(event.mouse.where);
        if (mp.x == 0 && mp.y >= 1 && mp.y <= size.y - 2) {
            int line = editor->lineAtRow(mp.y - 1);
            if (line >= 0) {
                editor->toggleFoldLine(line);
                clearEvent(event);
                return;
            }
        }
    }
    TFrame::handleEvent(event);
}

TFrame *NNWindow::initFrame(TRect r)
{
    return new NNFrame(r);
}

NNWindow::NNWindow(const TRect &bounds, TStringView fileName, int windowNum) noexcept
    : TWindowInit(&NNWindow::initFrame),
      TEditWindow(bounds, "", windowNum)
{
    // TEditWindow's constructor just created a plain empty TFileEditor.
    // Replace it with an NNEditor that loads UTF-8 content and adds highlighting.

    TScrollBar *h   = editor->hScrollBar;
    TScrollBar *v   = editor->vScrollBar;
    TIndicator *ind = editor->indicator;
    TRect r         = editor->getBounds();
    char fName[MAXPATH];
    strnzcpy(fName, fileName, sizeof(fName));

    // Remove and destroy the TFileEditor (leaves scrollbars and indicator intact)
    remove(editor);
    TObject::destroy(editor);

    // Insert the NNEditor in its place
    NNEditor *ne = new NNEditor(r, h, v, ind, fName);
    insert(ne);
    editor = ne;   // TEditWindow::editor is TFileEditor* — NNEditor IS-A TFileEditor

    static_cast<NNFrame*>(frame)->setEditor(ne);
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
