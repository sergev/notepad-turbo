# Key Bindings

## NotepadNext Reference Bindings

NotepadNext is Qt-based; shortcuts are defined in `MainWindow.ui` and `MainWindow.cpp`.
Custom shortcuts can also be set via `ApplicationSettings` (Shortcuts group).

### File

| Action | Shortcut |
|--------|----------|
| New | Ctrl+N |
| Open | Ctrl+O |
| Save | Ctrl+S |
| Save As | Ctrl+Alt+S |
| Save All | Ctrl+Shift+S |
| Close | Ctrl+W |
| Close All | Ctrl+Shift+W |
| Print | Ctrl+P |
| Restore Recently Closed | Ctrl+Shift+T |
| Exit | Alt+F4 / Cmd+Q |

### Edit

| Action | Shortcut |
|--------|----------|
| Undo | Ctrl+Z |
| Redo | Ctrl+Y |
| Cut | Ctrl+X |
| Copy | Ctrl+C |
| Paste | Ctrl+V |
| Delete | Del |
| Select All | Ctrl+A |
| Select Next Instance | Ctrl+D |
| Duplicate Current Line | Alt+Down |
| Move Lines Up | Ctrl+Shift+Up |
| Move Lines Down | Ctrl+Shift+Down |
| Join Lines | Ctrl+J |
| Toggle Single Line Comment | Ctrl+/ |
| Single Line Comment | Ctrl+K |
| Single Line Uncomment | Ctrl+Shift+K |

### Search & Navigation

| Action | Shortcut |
|--------|----------|
| Find | Ctrl+F |
| Find Next | F3 |
| Find Previous | Shift+F3 |
| Replace | Ctrl+H |
| Quick Find | Ctrl+Alt+I |
| Go to Line | Ctrl+G |
| Toggle Bookmark | Ctrl+F2 |
| Next Bookmark | F2 |
| Previous Bookmark | Shift+F2 |
| Search Results Dock | F7 |

### View

| Action | Shortcut |
|--------|----------|
| Full Screen | F11 |
| Zoom In | Ctrl++ |
| Zoom Out | Ctrl+- |
| Reset Zoom | Ctrl+0 |
| Next Tab | Ctrl+Tab / Ctrl+PageDown |
| Previous Tab | Ctrl+Shift+Tab / Ctrl+PageUp |

### Code Folding

| Action | Shortcut |
|--------|----------|
| Fold All | Alt+0 |
| Unfold All | Alt+Shift+0 |
| Fold Level 1–9 | Alt+1 … Alt+9 |
| Unfold Level 1–9 | Alt+Shift+1 … Alt+Shift+9 |

### Macro

| Action | Shortcut |
|--------|----------|
| Macro Playback | Ctrl+Shift+P |

### Help

| Action | Shortcut |
|--------|----------|
| About | F1 |

---

## Current Notepad Turbo Bindings

Defined in [src/NNMenus.cpp](../src/NNMenus.cpp) and [src/NNStatusLine.cpp](../src/NNStatusLine.cpp).
Commands are in [src/NNCommands.h](../src/NNCommands.h).

### Menu Bar Access

| Menu | Key |
|------|-----|
| File | Alt+F |
| Edit | Alt+E |
| Search | Alt+S |
| View | Alt+V |
| Tools | Alt+T |
| Window | Alt+W |
| Help | Alt+H |
| Open menu bar | F10 |

### File

| Action | Key | Command |
|--------|-----|---------|
| New | Ctrl+N | cmNNNew |
| Open | F3 | cmNNOpen |
| Save | F2 | cmNNSave |
| Save As | — | cmNNSaveAs |
| Close | Ctrl+W | cmNNClose |
| Close All | — | cmNNCloseAll |
| Exit | Ctrl+Q | cmQuit / Alt+X |

### Edit

| Action | Key | Command |
|--------|-----|---------|
| Undo | Ctrl+Z | cmUndo |
| Cut | Ctrl+X | cmCut |
| Copy | Ctrl+C | cmCopy |
| Paste | Ctrl+V | cmPaste |
| Delete | Del | cmClear |
| Select All | Ctrl+A | cmNNSelectAll |

### Search

| Action | Key | Command |
|--------|-----|---------|
| Find | Ctrl+F | cmNNFind |
| Replace | Ctrl+H | cmNNReplace |
| Find Next | F4 | cmNNFindNext |
| Find Previous | Shift+F4 | cmNNFindPrev (⚠ handler missing) |
| Go to Line | Ctrl+G | cmNNGotoLine |

### View

| Action | Key | Command |
|--------|-----|---------|
| Word Wrap | — | cmNNWordWrap |
| Whitespace | — | cmNNWhitespace |

### Tools / Macro

| Action | Key | Command |
|--------|-----|---------|
| Preferences | — | cmNNPreferences |
| Record Macro | — | cmNNMacroRecord (⚠ handler missing) |
| Stop Recording | — | cmNNMacroStop (⚠ handler missing) |
| Run Macro | — | cmNNMacroRun (⚠ handler missing) |
| Save Macro | — | cmNNMacroSave (⚠ handler missing) |

### Window

| Action | Key | Command |
|--------|-----|---------|
| Next Window | F6 | cmNext |
| Previous Window | Shift+F6 | cmPrev |
| Tile | — | cmTile |
| Cascade | — | cmCascade |
| Zoom window | F5 | cmZoom |

### Inherited Turbo Vision Editor Keys

These come from `TFileEditor` / `TEditor` and require no explicit binding.

| Action | Key |
|--------|-----|
| Move left/right | ←  → |
| Word left/right | Ctrl+←  Ctrl+→ |
| Move up/down | ↑  ↓ |
| Line start/end | Home  End |
| Page up/down | PgUp  PgDn |
| Document start/end | Ctrl+Home  Ctrl+End |
| Select + any move key | Shift+move |
| Help | F1 |

---

## Proposed Enhancement Plan

The table below maps each TODO.md item to a proposed key binding and implementation phase.
Phase 1 = bugs / already-implemented-but-unwired; Phase 2 = new features with
minimal new infrastructure; Phase 3 = significant new UI work.

### Phase 1 — Wire Up Existing Code (no new features needed)

| Action | Proposed Key | Command | Notes |
|--------|-------------|---------|-------|
| Find Previous | Shift+F4 | cmNNFindPrev | Add missing `case` in `handleEvent()` |
| Redo | Ctrl+Y | cmNNRedo (new) | Call `SCI_REDO`; add to Edit menu after Undo |
| Sort Lines Ascending | — | cmNNSortAsc | Add Line Operations submenu to Edit |
| Sort Lines Descending | — | cmNNSortDesc | Same submenu |
| Sort Lines Randomly | — | cmNNSortRandom | Same submenu |
| Toggle Line Numbers | — | cmNNLineNumbers | Add to View menu |
| Macro Record/Stop/Run/Save | — | cmNNMacro* | Add missing handlers in `handleEvent()` |
| Macro Pause | — | cmNNMacroPause | Add to Tools menu + handler |

### Phase 2 — Scintilla/Lua calls, new menu items

| Action | Proposed Key | Command | Notes |
|--------|-------------|---------|-------|
| Zoom In | Ctrl++ | cmNNZoomIn (new) | `SCI_ZOOMIN`; add to View |
| Zoom Out | Ctrl+- | cmNNZoomOut (new) | `SCI_ZOOMOUT`; add to View |
| Reset Zoom | Ctrl+0 | cmNNZoomReset (new) | `SCI_SETZOOM(0)`; add to View |
| Duplicate Line | Ctrl+D | cmNNDupLine (new) | `SCI_LINEDUPLICATE` |
| Move Line Up | Ctrl+Shift+↑ | cmNNMoveUp (new) | `SCI_MOVESELECTEDLINESUP` |
| Move Line Down | Ctrl+Shift+↓ | cmNNMoveDown (new) | `SCI_MOVESELECTEDLINESDOWN` |
| Toggle Comment | Ctrl+/ | cmNNComment (new) | Language-aware via Lua; language files already store comment strings |
| Convert to Uppercase | — | cmNNUpperCase (new) | `SCI_UPPERCASE`; Convert Case submenu |
| Convert to Lowercase | — | cmNNLowerCase (new) | `SCI_LOWERCASE`; Convert Case submenu |
| Toggle Whitespace | — | cmNNWhitespace | Already defined; verify `handleEvent()` |
| Toggle EOL chars | — | cmNNShowEOL (new) | Add to View |
| Toggle Indent Guides | — | cmNNIndentGuide (new) | Add to View |
| Save All | Ctrl+S | cmNNSaveAll (new) | Iterate open windows |
| Reload File | — | cmNNReload | Command already defined (206); wire handler |
| Close All Except Active | — | cmNNCloseOthers (new) | Add to File > Close More |
| EOL → Windows | — | cmNNEolWindows (new) | `SCI_SETEOLMODE(SC_EOL_CRLF)` |
| EOL → Unix | — | cmNNEolUnix (new) | `SCI_SETEOLMODE(SC_EOL_LF)` |
| EOL → Mac | — | cmNNEolMac (new) | `SCI_SETEOLMODE(SC_EOL_CR)` |

### Phase 3 — Significant new UI / infrastructure

| Action | Proposed Key | Notes |
|--------|-------------|-------|
| Toggle Fold (current line) | Ctrl+F5 | Fold infrastructure exists in `NNDocument`; need rendering + key handling |
| Fold All | Alt+0 | Match NotepadNext; dispatch via new command |
| Unfold All | Alt+Shift+0 | |
| Fold Level 1–9 | Alt+1…9 | |
| Unfold Level 1–9 | Alt+Shift+1…9 | |
| Bookmarks: Toggle | Ctrl+F2 | Needs bookmark data structure + margin indicator |
| Bookmarks: Next | F2 | |
| Bookmarks: Previous | Shift+F2 | |
| Find in Files | Ctrl+Shift+F | Requires new results pane |
| Column / Rectangular Select | Alt+drag | Turbo Vision support needed |

### Key Binding Conventions to Follow

- **Function keys** (F2–F6, F10) are used for the most frequent single-key actions
  visible in the status bar (Save, Open, Find Next, Zoom, Next Window, Menu).
- **Ctrl+letter** for common edit actions (matching NotepadNext where TUI allows).
- **Ctrl+Shift+letter** for the "reverse" or "all" variant of a Ctrl action
  (e.g., Ctrl+S = Save, Ctrl+Shift+S = Save All).
- **Alt+digit** reserved for fold levels (matching NotepadNext).
- Avoid **Alt+letter** combinations for hotkeys other than menu access, because
  Turbo Vision uses Alt+letter exclusively for menu navigation.
- Avoid **Ctrl+Tab / Ctrl+PageUp/Down** — terminal emulators often swallow or
  mangle these; use F6/Shift+F6 for window cycling instead.
