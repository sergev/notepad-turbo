# Notepad Turbo

A **terminal (TUI) text editor** in the spirit of Notepad++, built with **[Turbo Vision](https://github.com/magiblot/tvision)**. This project grows out of **[NotepadNext](https://github.com/dail8859/NotepadNext)**—Justin Dailey’s cross-platform reimplementation of Notepad++—which used a **Qt 6** GUI.

The application presents itself in-app as **Notepad Turbo** and is implemented as a Turbo Vision `TApplication` with Lexilla-powered highlighting and Lua-driven language definitions.

![screenshot](https://github.com/sergev/notepad-turbo/wiki/images/notepad-turbo.png)

## Features

- **Multi-document UI**: tile, cascade, next/previous window (see menus in [`src/NNMenus.cpp`](src/NNMenus.cpp)).
- **File**: new, open (F3), save (F2), save as, close, close all, exit.
- **Edit**: undo, cut, copy, paste, delete, select all.
- **Search**: find (Ctrl+F), replace (Ctrl+H), find next/prev (F4 / Shift+F4), go to line (Ctrl+G).
- **View**: word wrap, whitespace visibility.
- **Tools**: preferences; record / stop / run / save macros.
- **Syntax highlighting**: Lexilla lexers with styling driven from Lua (see [`src/NNEditor.h`](src/NNEditor.h), [`src/languages/*.lua`](src/languages)).
- **Encoding support**: detects file encodings with uchardet, edits internally as UTF-8, and saves back to the original encoding when possible. Preferences control whether unrepresentable characters fail the save, are replaced, or force UTF-8 output.
- **Persistence**: settings, recent files, and session restore (INI-style; see application code under [`src/`](src)).

## Requirements

- **CMake** ≥ 3.21
- **C++20** toolchain
- **Git** (Turbo Vision is fetched at configure time via `FetchContent`)
- Third-party code built in-tree: **Lua**, **Lexilla**, **uchardet**; Scintilla **headers** only (for Lexilla interfaces)

## Build

Using the top-level Makefile (creates `build/`, **RelWithDebInfo**):

```bash
make
```

The main executable target is **`nt`** ([`src/CMakeLists.txt`](src/CMakeLists.txt)).

Install (after a successful build):

```bash
make install
```

## Testing

```bash
make test
```

This builds and runs three test suites via CTest:

| Suite | What it covers |
| --- | --- |
| `unit_tests` | INI settings, sorter, session manager, recent files, macros, finder |
| `ui_tests` | File encoding (uchardet/iconv round-trips), NNEditor, NNWindow, NNDocument |
| `uchardet_tests` | Charset detection against 122 reference files (one `TEST()` per file) |

## Running

Run the built binary (typical path `build/nt`):

```bash
./build/nt
```

Adjust paths if your build output or working directory differs.

## License

This project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.txt) (see [`LICENSE`](LICENSE)). Third-party components under [`thirdparty/`](thirdparty) retain their own licenses (e.g. Lexilla, Scintilla headers, Lua, uchardet).

## Third-party / upstream

- **[Turbo Vision](https://github.com/magiblot/tvision)** — TUI framework (fetched by CMake; not vendored in-tree).
- **[NotepadNext](https://github.com/dail8859/NotepadNext)** — original Qt-based editor and shared lineage; **thanks again to Justin Dailey**.
