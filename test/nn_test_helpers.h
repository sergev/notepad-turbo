#pragma once

#define Uses_TGroup
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TRect
#define Uses_TScrollBar
#define Uses_TIndicator
#define Uses_TWindow
#define Uses_TFrame
#define Uses_TObject
#include <tvision/tv.h>
#include <tvision/colors.h>

#include <gtest/gtest.h>

#include "NNEditor.h"
#include "NNWindow.h"

#include <string>
#include <vector>

// TGroup subclass that allocates an in-memory screen buffer without a real
// terminal.  Insert a TView into it, call drawView() on that view, then read
// back the character and color grids.
//
// Usage:
//   DrawableTestGroup group({0,0}, {40,10});
//   group.insert(editor);
//   editor->drawView();
//   auto row = group.getRowText(0);
class DrawableTestGroup : public TGroup
{
public:
    DrawableTestGroup(TPoint origin, TPoint size) noexcept
        : TGroup(TRect(origin, size))
    {
        setState(sfExposed, True);
        getBuffer();
    }

    // Pass palette index through unchanged so tests can compare raw indices.
    TColorAttr mapColor(uchar i) noexcept override
    {
        return TColorAttr(i);
    }

    // Returns a [height][width] grid of UTF-8 strings (one per screen cell).
    std::vector<std::vector<std::string>> getBufferText() const
    {
        std::vector<std::vector<std::string>> text(
            size.y, std::vector<std::string>(size.x));
        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x) {
                const TCellChar &ch = buffer[y * size.x + x]._ch;
                if (ch._flags & TCellChar::fTrail)
                    text[y][x] = "(trail)";
                else
                    text[y][x] = std::string(ch.getText());
            }
        return text;
    }

    // Returns the TColorAttr at (x, y).
    TColorAttr getBufferAttr(int x, int y) const
    {
        return buffer[y * size.x + x].attr;
    }

    // Returns a single row as a plain ASCII string (best-effort; ignores wide chars).
    std::string getRowText(int y) const
    {
        std::string row;
        for (int x = 0; x < size.x; ++x) {
            const TCellChar &ch = buffer[y * size.x + x]._ch;
            if (!(ch._flags & TCellChar::fTrail))
                row += ch.getText();
        }
        return row;
    }
};

// RAII wrapper that owns a DrawableTestGroup and an NNEditor inside it.
// Call draw() to flush the editor's output to the buffer.
// Do not call draw() on tests that only need cursor/event state — the group
// is a plain TGroup, but NNEditor::draw() now uses dynamic_cast so it is safe.
struct NNEditorTest
{
    DrawableTestGroup *group = nullptr;
    NNEditor          *editor = nullptr;

    NNEditorTest() = default;
    NNEditorTest(const NNEditorTest&) = delete;
    NNEditorTest& operator=(const NNEditorTest&) = delete;

    void init(int w = 40, int h = 10)
    {
        TRect bounds({0, 0}, {w, h});
        group  = new DrawableTestGroup({0, 0}, {w, h});
        editor = new NNEditor(bounds, nullptr, nullptr, nullptr, "");
        group->insert(editor);
    }

    void draw() { editor->drawView(); }

    void insertText(const char *text)
    {
        editor->insertText(text, (uint)strlen(text), False);
    }

    void sendKey(ushort keyCode, ushort ctrlState = 0, TStringView text = {})
    {
        TEvent ev{};
        ev.what = evKeyDown;
        ev.keyDown.keyCode = keyCode;
        ev.keyDown.controlKeyState = ctrlState;
        ev.keyDown.textLength = 0;
        for (uchar i = 0; i < text.size() && i < sizeof(ev.keyDown.text); ++i) {
            ev.keyDown.text[i] = text[i];
            ev.keyDown.textLength = i + 1;
        }
        editor->handleEvent(ev);
    }

    ~NNEditorTest()
    {
        if (group) {
            TObject::destroy(group);
            group  = nullptr;
            editor = nullptr;
        }
    }
};

// RAII wrapper for NNWindow tests (title, frame markers, etc.).
// NNWindow is inserted into a DrawableTestGroup acting as the desktop.
struct NNWindowTest
{
    DrawableTestGroup *desktop = nullptr;
    NNWindow          *win     = nullptr;

    NNWindowTest() = default;
    NNWindowTest(const NNWindowTest&) = delete;
    NNWindowTest& operator=(const NNWindowTest&) = delete;

    void init(int w = 42, int h = 12, const char *name = "test.txt")
    {
        desktop = new DrawableTestGroup({0, 0}, {w, h});
        win     = new NNWindow(TRect({0, 0}, {w, h}), name, 1);
        desktop->insert(win);
        desktop->redraw();
    }

    NNEditor *editor() const { return win->nnEditor(); }

    void draw() { desktop->redraw(); }

    ~NNWindowTest()
    {
        if (desktop) {
            TObject::destroy(desktop);
            desktop = nullptr;
            win     = nullptr;
        }
    }
};

// Build a key-down TEvent.
inline TEvent keyDownEv(ushort keyCode, ushort controlKeyState = 0,
                         TStringView text = {})
{
    TEvent ev{};
    ev.what = evKeyDown;
    ev.keyDown.keyCode         = keyCode;
    ev.keyDown.controlKeyState = controlKeyState;
    ev.keyDown.textLength      = 0;
    for (uchar i = 0; i < text.size() && i < sizeof(ev.keyDown.text); ++i) {
        ev.keyDown.text[i]    = text[i];
        ev.keyDown.textLength = i + 1;
    }
    return ev;
}

// Build a message TEvent.
inline TEvent messageEv(ushort what, ushort command, void *infoPtr = nullptr)
{
    TEvent ev{};
    ev.what               = what;
    ev.message.command    = command;
    ev.message.infoPtr    = infoPtr;
    return ev;
}
