# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Notepad Turbo** (`nt`) is a terminal UI text editor written in C++20, inspired by Notepad++. It runs in the terminal using the Turbo Vision TUI framework and supports syntax highlighting via Lexilla, with language definitions written in Lua.

## Build & Run

```bash
make              # Builds in RelWithDebInfo (output: build/nt)
make debug        # Builds in Debug mode
make test         # Builds and runs all three test suites via ctest:
                  #   unit_tests    — IniSettings, Sorter, Session, Macros, Finder
                  #   ui_tests      — FileEncoding, NNEditor, NNWindow, NNDocument
                  #   uchardet_tests — charset detection (122 GoogleTest cases)
make install      # Installs binary + Lua scripts to /usr/local/
make clean        # Removes build/ directory
./build/nt        # Run the editor
```

The Makefile wraps CMake. Build artifacts go in `build/`. The compiler enforces `-Wall -Werror`, so all warnings are errors.

## Architecture

### Core Components

**Application entry point**
- `main.cpp` → `NNApplication` (`NNApplication.h/cpp`) — TApplication subclass; owns menus, status bar, desktop, and event loop

**Editor stack** (bottom-up):
1. `NNDocument` — adapts TFileEditor's internal gap buffer to the `Scintilla::IDocument` interface so Lexilla can tokenize it
2. `NNEditor` — TFileEditor subclass; integrates Lexilla for per-character syntax coloring, implements `IEditorAPI`, handles macro recording/playback
3. `NNWindow` — TEditWindow wrapping NNEditor; manages window title and lifecycle
4. `NNEditorManager` — coordinates multiple open windows (tile, cascade, tab switching)

**Lua / syntax highlighting pipeline**
- `LuaState` wraps the Lua VM; `LuaExtension` exports C++ editor APIs to Lua scripts
- `IFaceTable` / `SciIFaceTable` / `IFaceTableMixer` map Scintilla's lexer interface to Lua tables
- `src/scripts/init.lua` bootstraps the scripting layer and loads all language definitions
- `src/languages/*.lua` — 88 data-driven language files (keywords, file extensions, comment styles, colors)

**Supporting subsystems**
- `FileEncoding` — detects source encodings with uchardet/iconv, decodes file bytes to internal UTF-8, remembers source charset/BOM metadata, and re-encodes on save according to the global `Editor.encoding_save_policy`
- `Finder` — search/replace (regex, case, whole-word, replace-all)
- `MacroRecorder` / `MacroStep` / `Macro` / `MacroManager` — keystroke recording and replay
- `IniSettings` — simple key=value / section.key INI settings store
- `SessionManager` — persists and restores open files and window layout across sessions
- `RecentFilesListManager` — tracks recently opened files
- `Sorter` — text sorting utility
- `src/dialogs/` — find/replace, goto-line, preferences, macro save/run dialogs

### Key Interfaces

- `IEditorAPI` (`IEditorAPI.h`) — interface between the editor and Lexilla styler (colors, bold/italic, keywords, properties)
- `Scintilla::IDocument` — implemented by `NNDocument` to let Lexilla read the gap buffer
- `FileEncoding::DecodeResult` / `SourceEncoding` — carry decoded UTF-8 text plus original encoding metadata so `NNEditor` can save back to the original file encoding when possible

### Third-Party Dependencies

Managed by CMake FetchContent (not vendored except `thirdparty/`):
- **Turbo Vision** — TUI framework (fetched at configure time)
- **Lua** — embedded in `thirdparty/lua/`
- **Lexilla** — syntax analysis in `thirdparty/lexilla/`
- **Scintilla** — headers only (ILexer, IDocument) in `thirdparty/scintilla/`
- **uchardet** — encoding detection in `thirdparty/uchardet/`

### Naming Conventions

- Classes prefixed with `NN` are application-level
- Language `.lua` files are the primary extension point for adding new syntax support — no C++ changes needed
