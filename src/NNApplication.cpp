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

#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TProgram
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TRect
#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TFileDialog
#define Uses_MsgBox
#define Uses_TObject
#define Uses_TPalette
#include <tvision/tv.h>

#include "NNApplication.h"
#include "NNMenus.h"
#include "NNCommands.h"
#include "NNWindow.h"
#include "NNEditor.h"
#include "NNEditorManager.h"
#include "IniSettings.h"
#include "RecentFilesListManager.h"
#include "LuaState.h"
#include "LuaExtension.h"

#include "dialogs/NNFindReplaceDialog.h"
#include "dialogs/NNGotoLineDialog.h"
#include "dialogs/NNPreferencesDialog.h"

#include <filesystem>
#include <cstdlib>
#include <memory>

namespace {

namespace fs = std::filesystem;

// Pick a Lua scripts root under an install or data prefix (e.g. .../share/notepad-turbo).
// CMake installs init.lua under PREFIX/share/notepad-turbo/scripts/.
static fs::path scriptsRootFromBase(const fs::path &base)
{
    if (!fs::exists(base) || !fs::is_directory(base))
        return {};

    const fs::path scriptsSub = base / "scripts";
    if (fs::is_directory(scriptsSub) && fs::exists(scriptsSub / "init.lua"))
        return scriptsSub;

    if (fs::exists(base / "init.lua"))
        return base;

    return {};
}

static std::string resolveDefaultScriptsDir()
{
    // Development: repo layout src/scripts/init.lua (run from project root or build/)
    {
        const fs::path devSrc = fs::path("src") / "scripts";
        if (fs::exists(devSrc / "init.lua"))
            return fs::weakly_canonical(devSrc).string();
    }
    {
        const fs::path fromBuild = fs::path("..") / "src" / "scripts";
        if (fs::exists(fromBuild / "init.lua"))
            return fs::weakly_canonical(fromBuild).string();
    }
    // Development: cwd-relative scripts/ with init.lua
    {
        const fs::path dev = "scripts";
        if (fs::is_directory(dev) && fs::exists(dev / "init.lua"))
            return dev.string();
    }

    if (const char *home = std::getenv("HOME")) {
        const fs::path p = scriptsRootFromBase(fs::path(home) / ".local" / "share" / "notepad-turbo");
        if (!p.empty())
            return p.string();
    }

    for (const char *base : {
             "/usr/local/share/notepad-turbo",
             "/opt/local/share/notepad-turbo",
         }) {
        const fs::path p = scriptsRootFromBase(fs::path(base));
        if (!p.empty())
            return p.string();
    }

    return "scripts";
}

} // namespace

// Helper: run a dialog on the desktop
static ushort runDialog(TDialog *d, void *data = nullptr)
{
    TView *p = TProgram::application->validView(d);
    if (!p) return cmCancel;
    if (data) p->setData(data);
    ushort result = TProgram::deskTop->execView(p);
    if (result != cmCancel && data) p->getData(data);
    TObject::destroy(p);
    return result;
}

// Standard editor dialog callback (Find/Replace/Save prompts)
static ushort editorDialogCallback(int dialog, ...)
{
    va_list arg;
    char buf[512] = {};

    switch (dialog) {
        case edOutOfMemory:
            return messageBox("Not enough memory for this operation.", mfError | mfOKButton);

        case edReadError: {
            va_start(arg, dialog);
            snprintf(buf, sizeof(buf), "Error reading file %s.", va_arg(arg, const char*));
            va_end(arg);
            return messageBox(buf, mfError | mfOKButton);
        }
        case edWriteError: {
            va_start(arg, dialog);
            snprintf(buf, sizeof(buf), "Error writing file %s.", va_arg(arg, const char*));
            va_end(arg);
            return messageBox(buf, mfError | mfOKButton);
        }
        case edCreateError: {
            va_start(arg, dialog);
            snprintf(buf, sizeof(buf), "Error creating file %s.", va_arg(arg, const char*));
            va_end(arg);
            return messageBox(buf, mfError | mfOKButton);
        }
        case edSaveModify: {
            va_start(arg, dialog);
            snprintf(buf, sizeof(buf), "%s has been modified. Save?", va_arg(arg, const char*));
            va_end(arg);
            return messageBox(buf, mfInformation | mfYesNoCancel);
        }
        case edSaveUntitled:
            return messageBox("Save untitled file?", mfInformation | mfYesNoCancel);

        case edSaveAs: {
            va_start(arg, dialog);
            char *fileName = va_arg(arg, char*);
            va_end(arg);
            return runDialog(
                new TFileDialog("*.*", "Save file as", "~N~ame", fdOKButton, 101),
                fileName);
        }
        case edFind: {
            va_start(arg, dialog);
            void *data = va_arg(arg, void*);
            va_end(arg);
            return runDialog(NNFindReplaceDialog::createFind(), data);
        }
        case edSearchFailed:
            return messageBox("Search string not found.", mfError | mfOKButton);

        case edReplace: {
            va_start(arg, dialog);
            void *data = va_arg(arg, void*);
            va_end(arg);
            return runDialog(NNFindReplaceDialog::createReplace(), data);
        }
        case edReplacePrompt:
            return messageBox("Replace this occurrence?", mfYesNoCancel | mfInformation);

        default:
            return cmCancel;
    }
}

NNApplication::NNApplication(int argc, char **argv)
    : TProgInit(&NNApplication::initStatusLine,
                &NNApplication::initMenuBar,
                &TProgram::initDeskTop),
      TApplication()
{
    TEditor::editorDialog = editorDialogCallback;

    // Disable commands that require an open file until one is opened
    TCommandSet ts;
    ts.enableCmd(cmNNSave);
    ts.enableCmd(cmNNSaveAs);
    ts.enableCmd(cmNNClose);
    ts.enableCmd(cmNNCloseAll);
    ts.enableCmd(cmNNFind);
    ts.enableCmd(cmNNReplace);
    ts.enableCmd(cmNNFindNext);
    ts.enableCmd(cmNNFindPrev);
    ts.enableCmd(cmNNGotoLine);
    ts.enableCmd(cmNNFoldToggle);
    ts.enableCmd(cmNNFoldAll);
    ts.enableCmd(cmNNUnfoldAll);
    ts.enableCmd(cmSave);
    ts.enableCmd(cmSaveAs);
    ts.enableCmd(cmCut);
    ts.enableCmd(cmCopy);
    ts.enableCmd(cmPaste);
    ts.enableCmd(cmClear);
    ts.enableCmd(cmUndo);
    ts.enableCmd(cmFind);
    ts.enableCmd(cmReplace);
    ts.enableCmd(cmSearchAgain);
    disableCommands(ts);

    // Initialize settings
    std::string configDir = IniSettings::defaultConfigDir("notepad-turbo");
    settings = std::make_unique<IniSettings>(configDir + "/settings.ini");
    settings->load();

    recentFiles = std::make_unique<RecentFilesListManager>(settings.get());
    recentFiles->load();

    editorManager = std::make_unique<NNEditorManager>();

    initLua();

    // Open files passed on command line
    for (int i = 1; i < argc; ++i)
        openEditorWindow(argv[i]);

    // Restore session if enabled and no files were opened on command line
    if (argc <= 1 && settings->getBool("Session.restore_previous", true))
        loadSession();
}

NNApplication::~NNApplication()
{
    saveSession();
    recentFiles->save();
    settings->save();
}

void NNApplication::initLua()
{
    lua = std::make_unique<LuaState>();

    std::string scriptDir;
    const char *scriptEnv = getenv("NN_SCRIPTS_DIR");
    if (scriptEnv)
        scriptDir = scriptEnv;
    else
        scriptDir = resolveDefaultScriptsDir();

    lua->setScriptsDir(scriptDir);
    LuaExtension::Instance().Initialise(lua->L, nullptr);
    lua->executeInitScript();
}

TMenuBar *NNApplication::initMenuBar(TRect r)
{
    return createNNMenuBar(r);
}

TStatusLine *NNApplication::initStatusLine(TRect r)
{
    return createNNStatusLine(r);
}

TPalette &NNApplication::getPalette() const
{
    static const char appPalette[] = {
        // TBackground.
        0x08, // 1: fill pattern

        // Shared menu bar, menu box, status line, and indicator colors.
        0x30, // 2: normal text / first indicator color
        0x38, // 3: disabled text / second indicator color
        0x34, // 4: shortcut text
        0x20, // 5: normal selection
        0x28, // 6: disabled selection
        0x24, // 7: shortcut selection

        // Black TWindow.
        0x08, // 8: frame passive
        0x03, // 9: frame active
        0x0A, // 10: frame icons
        0x30, // 11: scroll bar page
        0x30, // 12: scroll bar controls
        0x07, // 13: scroller normal / static text default
        0x17, // 14: scroller selected
        0x0F, // 15: reserved

        // Cyan TWindow.
        0x37, // 16: frame passive
        0x3F, // 17: frame active
        0x3A, // 18: frame icons
        0x13, // 19: scroll bar page
        0x13, // 20: scroll bar controls
        0x3E, // 21: scroller normal
        0x21, // 22: scroller selected
        0x3F, // 23: reserved

        // Gray TWindow.
        0x70, // 24: frame passive
        0x7F, // 25: frame active
        0x7A, // 26: frame icons
        0x13, // 27: scroll bar page
        0x13, // 28: scroll bar controls
        0x70, // 29: scroller normal
        0x7F, // 30: scroller selected
        0x7E, // 31: reserved

        // Gray TDialog.
        0x70, // 32: frame passive
        0x7F, // 33: frame active
        0x7A, // 34: frame icon
        0x13, // 35: scroll bar page
        0x13, // 36: scroll bar controls
        0x70, // 37: static text
        0x70, // 38: label normal
        0x7F, // 39: label selected
        0x7E, // 40: label shortcut
        0x20, // 41: button normal
        0x2B, // 42: button default
        0x2F, // 43: button selected
        0x78, // 44: button disabled
        0x2E, // 45: button shortcut
        0x70, // 46: button shadow
        0x30, // 47: cluster normal
        0x3F, // 48: cluster selected
        0x3E, // 49: cluster shortcut
        0x1F, // 50: input line normal
        0x2F, // 51: input line selected
        0x1A, // 52: input line arrows
        0x20, // 53: history arrow
        0x72, // 54: history sides
        0x31, // 55: history window scroll page
        0x31, // 56: history window scroll controls
        0x30, // 57: list viewer normal
        0x2F, // 58: list viewer focused
        0x3E, // 59: list viewer selected
        0x31, // 60: list viewer divider
        0x13, // 61: file info pane
        0x38, // 62: cluster disabled
        0x00, // 63: reserved

        // Blue TDialog.
        0x17, // 64: frame passive
        0x1F, // 65: frame active
        0x1A, // 66: frame icon
        0x71, // 67: scroll bar page
        0x71, // 68: scroll bar controls
        0x1E, // 69: static text
        0x17, // 70: label normal
        0x1F, // 71: label selected
        0x1E, // 72: label shortcut
        0x20, // 73: button normal
        0x2B, // 74: button default
        0x2F, // 75: button selected
        0x78, // 76: button disabled
        0x2E, // 77: button shortcut
        0x10, // 78: button shadow
        0x30, // 79: cluster normal
        0x3F, // 80: cluster selected
        0x3E, // 81: cluster shortcut
        0x70, // 82: input line normal
        0x2F, // 83: input line selected
        0x7A, // 84: input line arrows
        0x20, // 85: history arrow
        0x12, // 86: history sides
        0x31, // 87: history window scroll page
        0x31, // 88: history window scroll controls
        0x30, // 89: list viewer normal
        0x2F, // 90: list viewer focused
        0x3E, // 91: list viewer selected
        0x31, // 92: list viewer divider
        0x13, // 93: file info pane
        0x38, // 94: cluster disabled
        0x00, // 95: reserved

        // Cyan TDialog.
        0x37, // 96: frame passive
        0x3F, // 97: frame active
        0x3A, // 98: frame icon
        0x13, // 99: scroll bar page
        0x13, // 100: scroll bar controls
        0x3E, // 101: static text
        0x30, // 102: label normal
        0x3F, // 103: label selected
        0x3E, // 104: label shortcut
        0x20, // 105: button normal
        0x2B, // 106: button default
        0x2F, // 107: button selected
        0x78, // 108: button disabled
        0x2E, // 109: button shortcut
        0x30, // 110: button shadow
        0x70, // 111: cluster normal
        0x7F, // 112: cluster selected
        0x7E, // 113: cluster shortcut
        0x1F, // 114: input line normal
        0x2F, // 115: input line selected
        0x1A, // 116: input line arrows
        0x20, // 117: history arrow
        0x32, // 118: history sides
        0x31, // 119: history window scroll page
        0x71, // 120: history window scroll controls
        0x70, // 121: list viewer normal
        0x2F, // 122: list viewer focused
        0x7E, // 123: list viewer selected
        0x71, // 124: list viewer divider
        0x13, // 125: file info pane
        0x78, // 126: cluster disabled
        0x00, // 127: reserved

        // THelpWindow.
        0x37, // 128: frame passive
        0x3F, // 129: frame active
        0x3A, // 130: frame icon
        0x13, // 131: scroll bar page
        0x13, // 132: scroll bar controls
        0x30, // 133: help viewer normal text
        0x3E, // 134: help viewer keyword
        0x1E, // 135: help viewer selected keyword
    };

    static TPalette palette( appPalette, sizeof(appPalette) );
    return palette;
}

NNWindow *NNApplication::openEditorWindow(const std::string &path)
{
    TRect r = deskTop->getExtent();
    NNWindow *win = new NNWindow(r, path.empty() ? "" : path.c_str(), nextWindowNum++);

    TView *p = validView(win);
    if (!p)
        return nullptr;

    deskTop->insert(p);

    NNEditor *ed = win->nnEditor();

    // Enable file-related commands now that we have an editor
    TCommandSet ts;
    ts.enableCmd(cmNNSave);
    ts.enableCmd(cmNNSaveAs);
    ts.enableCmd(cmNNClose);
    ts.enableCmd(cmNNCloseAll);
    ts.enableCmd(cmNNFind);
    ts.enableCmd(cmNNReplace);
    ts.enableCmd(cmNNFindNext);
    ts.enableCmd(cmNNFindPrev);
    ts.enableCmd(cmNNGotoLine);
    ts.enableCmd(cmNNFoldToggle);
    ts.enableCmd(cmNNFoldAll);
    ts.enableCmd(cmNNUnfoldAll);
    ts.enableCmd(cmSave);
    ts.enableCmd(cmSaveAs);
    ts.enableCmd(cmCut);
    ts.enableCmd(cmCopy);
    ts.enableCmd(cmPaste);
    ts.enableCmd(cmClear);
    ts.enableCmd(cmUndo);
    ts.enableCmd(cmFind);
    ts.enableCmd(cmReplace);
    ts.enableCmd(cmSearchAgain);
    enableCommands(ts);

    // Apply language / syntax highlighting
    if (!path.empty()) {
        recentFiles->addFile(path);
        editorManager->applyLanguage(ed, path, lua.get());
    }

    return win;
}

NNWindow *NNApplication::currentWindow()
{
    TView *v = deskTop->current;
    if (!v) return nullptr;
    return dynamic_cast<NNWindow*>(v);
}

// --- File operations ---

void NNApplication::fileNew()
{
    openEditorWindow("");
}

void NNApplication::fileOpen()
{
    char fileName[MAXPATH] = {};
    if (runDialog(new TFileDialog("*.*", "Open File", "~N~ame", fdOpenButton, 100), fileName) != cmCancel)
        openEditorWindow(fileName);
}

void NNApplication::fileOpenPath(const std::string &path)
{
    openEditorWindow(path);
}

void NNApplication::fileSave()
{
    NNWindow *win = currentWindow();
    if (win)
        win->nnEditor()->save();
}

void NNApplication::fileSaveAs()
{
    NNWindow *win = currentWindow();
    if (win)
        win->nnEditor()->saveAs();
}

void NNApplication::fileClose()
{
    NNWindow *win = currentWindow();
    if (win)
        win->close();
}

void NNApplication::fileCloseAll()
{
    // Iterate in reverse so closing doesn't shift indices
    auto wins = editorManager->windows();
    for (auto it = wins.rbegin(); it != wins.rend(); ++it)
        (*it)->close();
}

// --- Search ---

void NNApplication::showFindDialog()
{
    NNWindow *win = currentWindow();
    if (!win) return;
    win->nnEditor()->find();
}

void NNApplication::showReplaceDialog()
{
    NNWindow *win = currentWindow();
    if (!win) return;
    win->nnEditor()->replace();
}

void NNApplication::showGotoLine()
{
    NNWindow *win = currentWindow();
    if (!win) return;

    NNGotoLineDialog dlg;
    int line = dlg.run();
    if (line >= 1) {
        NNEditor *ed = win->nnEditor();
        // Navigate to line (1-based)
        uint ptr = ed->lineMove(0, line - 1);
        ed->setCurPtr(ptr, 0);
        ed->trackCursor(true);
    }
}

// --- Preferences ---

void NNApplication::showPreferences()
{
    NNPreferencesDialog dlg(settings.get());
    dlg.run();
}

// --- About ---

void NNApplication::showAbout()
{
    messageBox(
        "\n"
        "  Notepad Turbo TUI\n"
        "  Version " APP_VERSION "\n"
        "\n"
        "  A cross-platform text editor\n"
        "  built on Turbo Vision\n"
        "\n"
        "  " APP_COPYRIGHT "\n",
        mfInformation | mfOKButton);
}

// --- Session ---

void NNApplication::loadSession()
{
    int count = settings->getInt("Session.count", 0);
    for (int i = 0; i < count; ++i) {
        std::string path = settings->get("Session.file." + std::to_string(i));
        if (!path.empty() && std::filesystem::exists(path))
            openEditorWindow(path);
    }
}

void NNApplication::saveSession()
{
    settings->removeSection("Session");
    const auto &wins = editorManager->windows();
    settings->setInt("Session.count", (int)wins.size());
    for (int i = 0; i < (int)wins.size(); ++i) {
        std::string path = wins[i]->nnEditor()->fileName;
        if (!path.empty())
            settings->set("Session.file." + std::to_string(i), path);
    }
}

// --- Event handling ---

void NNApplication::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what != evCommand)
        return;

    switch (event.message.command) {
        case cmNNNew:      fileNew();             break;
        case cmNNOpen:     fileOpen();            break;
        case cmNNSave:     fileSave();            break;
        case cmNNSaveAs:   fileSaveAs();          break;
        case cmNNClose:    fileClose();           break;
        case cmNNCloseAll: fileCloseAll();        break;
        case cmNNFind:     showFindDialog();      break;
        case cmNNReplace:  showReplaceDialog();   break;
        case cmNNFindNext: {
            NNWindow *win = currentWindow();
            if (win) win->nnEditor()->doSearchReplace();
            break;
        }
        case cmNNGotoLine: showGotoLine();        break;
        case cmNNPreferences: showPreferences();  break;
        case cmNNAbout:    showAbout();           break;
        case cmNNFoldToggle: {
            NNWindow *win = currentWindow();
            if (win) win->nnEditor()->toggleFold();
            break;
        }
        case cmNNFoldAll: {
            NNWindow *win = currentWindow();
            if (win) win->nnEditor()->foldAll(true);
            break;
        }
        case cmNNUnfoldAll: {
            NNWindow *win = currentWindow();
            if (win) win->nnEditor()->foldAll(false);
            break;
        }
        default:
            return;
    }
    clearEvent(event);
}

void NNApplication::idle()
{
    TApplication::idle();
}
