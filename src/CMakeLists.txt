#
# TUI-independent core library (no Turbo Vision dependency).
# Linked by unit_tests and the nt executable.
#
add_library(nt_core STATIC
    ITextBuffer.h
    IniSettings.h         IniSettings.cpp
    Sorter.h              Sorter.cpp
    SessionManager.h      SessionManager.cpp
    RecentFilesListManager.h RecentFilesListManager.cpp
    MacroStep.h           MacroStep.cpp
    Macro.h               Macro.cpp
    MacroRecorder.h       MacroRecorder.cpp
    MacroManager.h        MacroManagerSettings.cpp
    Finder.h              Finder.cpp
)

target_include_directories(nt_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
# TUI editor library: NNEditor + NNDocument + NNWindow.
# Depends on tvision::tvision and lexilla (no Lua).
# Linked by both nt executable and ui_tests.
#
add_library(nt_ui STATIC
    NNCommands.h
    IEditorAPI.h
    NNEditor.h      NNEditor.cpp
    NNDocument.h    NNDocument.cpp
    NNWindow.h      NNWindow.cpp
)
target_include_directories(nt_ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/thirdparty/scintilla/include
    ${CMAKE_SOURCE_DIR}/thirdparty/lexilla/include
    ${CMAKE_SOURCE_DIR}/thirdparty/lexilla/access
)
target_link_libraries(nt_ui PUBLIC nt_core tvision::tvision lexilla)

add_executable(nt
    main.cpp

    # Application
    NNApplication.h NNApplication.cpp
    NNMenus.h NNMenus.cpp
    NNStatusLine.h NNStatusLine.cpp

    # Editor (remaining TUI classes not in nt_ui)
    NNEditorManager.h NNEditorManager.cpp

    # Lua language system
    LuaState.h LuaState.cpp
    LuaExtension.h LuaExtension.cpp
    IFaceTable.h IFaceTable.cpp
    IFaceTableMixer.h IFaceTableMixer.cpp
    SciIFaceTable.h SciIFaceTable.cpp

    # Macros (TUI-dependent replay)
    MacroReplay.cpp
    MacroManager.h MacroManager.cpp

    # Dialogs
    dialogs/NNFindReplaceDialog.h dialogs/NNFindReplaceDialog.cpp
    dialogs/NNGotoLineDialog.h dialogs/NNGotoLineDialog.cpp
    dialogs/NNPreferencesDialog.h dialogs/NNPreferencesDialog.cpp
    dialogs/NNMacroRunDialog.h dialogs/NNMacroRunDialog.cpp
    dialogs/NNMacroSaveDialog.h dialogs/NNMacroSaveDialog.cpp
)

target_include_directories(nt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs
)

target_link_libraries(nt
    PRIVATE
        nt_ui
        lua
        uchardet
)

target_compile_definitions(nt
    PRIVATE
        APP_VERSION="${PROJECT_VERSION}"
        APP_COPYRIGHT="${APP_COPYRIGHT}"
)

install(TARGETS nt
    DESTINATION bin
)

# Install Lua init + language definitions (LuaState loads languages/*.lua next to init.lua)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
    DESTINATION share/notepad-turbo/scripts
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/languages/
    DESTINATION share/notepad-turbo/scripts/languages
)
