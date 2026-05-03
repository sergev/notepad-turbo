/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
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

#include "dialogs/NNFindReplaceDialog.h"
#include "dialogs/NNGotoLineDialog.h"
#include "dialogs/NNPreferencesDialog.h"

#include <filesystem>
#include <cstdlib>

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

    editorManager = std::make_unique<NNEditorManager>(settings.get());

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
        "  Notepad Next TUI\n"
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
        default:
            return;
    }
    clearEvent(event);
}

void NNApplication::idle()
{
    TApplication::idle();
}
