add_executable(nt
    main.cpp

    # Application
    NNApplication.h NNApplication.cpp
    NNCommands.h
    NNMenus.h NNMenus.cpp
    NNStatusLine.h NNStatusLine.cpp
    IEditorAPI.h

    # Editor
    NNEditor.h NNEditor.cpp
    NNDocument.h NNDocument.cpp
    NNWindow.h NNWindow.cpp
    NNEditorManager.h NNEditorManager.cpp

    # Settings & persistence
    IniSettings.h IniSettings.cpp
    RecentFilesListManager.h RecentFilesListManager.cpp
    SessionManager.h SessionManager.cpp

    # Lua language system
    LuaState.h LuaState.cpp
    LuaExtension.h LuaExtension.cpp
    IFaceTable.h IFaceTable.cpp
    IFaceTableMixer.h IFaceTableMixer.cpp
    SciIFaceTable.h SciIFaceTable.cpp

    # Search
    Finder.h Finder.cpp

    # Sorting
    Sorter.h Sorter.cpp

    # Macros
    Macro.h Macro.cpp
    MacroStep.h MacroStep.cpp
    MacroManager.h MacroManager.cpp
    MacroRecorder.h MacroRecorder.cpp

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
        ${CMAKE_SOURCE_DIR}/thirdparty/scintilla/include
        ${CMAKE_SOURCE_DIR}/thirdparty/lexilla/include
        ${CMAKE_SOURCE_DIR}/thirdparty/lexilla/access
)

target_link_libraries(nt
    PRIVATE
        tvision::tvision
        lexilla
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

# Install language scripts alongside the binary
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
    DESTINATION share/notepad-turbo/scripts
)
