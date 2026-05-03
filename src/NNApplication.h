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

#define Uses_TApplication
#define Uses_TMenuBar
#define Uses_TStatusLine
#define Uses_TDeskTop
#define Uses_TRect
#define Uses_TEvent
#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TEditWindow
#define Uses_TPalette
#include <tvision/tv.h>

#include <memory>
#include <string>
#include <vector>

class IniSettings;
class RecentFilesListManager;
class NNEditorManager;
class NNWindow;
class LuaState;

class NNApplication : public TApplication {
public:
    NNApplication(int argc, char **argv);
    ~NNApplication();

    void handleEvent(TEvent &event) override;
    void idle() override;

    TPalette &getPalette() const override;

    static TMenuBar    *initMenuBar(TRect r);
    static TStatusLine *initStatusLine(TRect r);

    // File operations
    void fileNew();
    void fileOpen();
    void fileOpenPath(const std::string &path);
    void fileSave();
    void fileSaveAs();
    void fileClose();
    void fileCloseAll();

    // Search
    void showFindDialog();
    void showReplaceDialog();
    void showGotoLine();

    // Preferences
    void showPreferences();

    // About
    void showAbout();

private:
    std::unique_ptr<IniSettings>          settings;
    std::unique_ptr<RecentFilesListManager> recentFiles;
    std::unique_ptr<NNEditorManager>       editorManager;
    std::unique_ptr<LuaState>              lua;

    int nextWindowNum = 1;

    NNWindow *openEditorWindow(const std::string &path);
    NNWindow *currentWindow();

    void loadSession();
    void saveSession();
    void initLua();
};
