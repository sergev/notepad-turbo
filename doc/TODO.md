# TODO: Features to Expose or Implement

This document tracks features that are either already implemented in the codebase but not connected to any menu item or key binding, or are present in NotepadNext (the upstream Qt-based editor) and worth porting.

---

## Implemented in Code but Not Exposed to the User

### Sorting (`Sorter.h/cpp`)
Full `Sorter` class hierarchy exists with ascending, descending, case-insensitive, length-based,
and random variants. Commands are defined in `NNCommands.h` but not wired in `NNApplication::handleEvent()`
and not added to any menu.

- Sort Lines Ascending (cmNNSortAsc = 250)
- Sort Lines Descending (cmNNSortDesc = 251)
- Sort Lines Randomly (cmNNSortRandom = 252)

### Line Numbers Toggle
Command `cmNNLineNumbers` (220) is defined in `NNCommands.h`. The margin infrastructure is in place
(MarginWidthN[1] for line numbers configured via Lua), but there is no menu item or handler.

### Macro Pause
`cmNNMacroPause` (233) is defined alongside the other macro commands but is never added to the
macro menu or handled. Record/Stop/Run/Save are wired; Pause is not.

### Code Folding (`NNDocument.cpp`)
The entire fold system is implemented:
- Fold levels are tracked per line in `NNDocument` (`foldLevel` vector).
- The Lua `init.lua` sets `"fold" = "1"`, enabling fold computation in Lexilla.
- The fold margin (`MarginWidthN[2]`) is configured.
- Scintilla IFace functions `ToggleFold`, `FoldAll`, `FoldChildren`, `FoldLine` etc. are available.

Nothing exposes these to the user. Suggested menu entries:
- Toggle Fold (current line)
- Fold All / Unfold All
- Fold Level 1–9 / Unfold Level 1–9

### Find Previous
The menu item "Find Previous" (Shift-F4) exists in `NNMenus.cpp` and `cmNNFindPrev` is defined,
but there is no case for it in `NNApplication::handleEvent()`. The menu item is therefore a no-op.

### Macro Recording and Replay
`MacroRecorder`, `MacroStep`, `Macro`, and `MacroManager` are fully implemented. The menu items
for Record / Stop Recording / Run Macro / Save Macro are present in the menu, but the corresponding
commands are not handled in `NNApplication::handleEvent()`.

---

## Available via Scintilla/Lua but Not Exposed

These operations are accessible through the Scintilla IFace table (already bridged to Lua) but
have no menu items or key bindings.

| Feature | Scintilla Message |
|---|---|
| Convert to Uppercase | SCI_UPPERCASE |
| Convert to Lowercase | SCI_LOWERCASE |
| Duplicate Current Line | SCI_LINEDUPLICATE |
| Redo | SCI_REDO / SCI_CANREDO |
| Zoom In | SCI_ZOOMIN |
| Zoom Out | SCI_ZOOMOUT |
| Reset Zoom | SCI_SETZOOM(0) |

---

## NotepadNext Features Worth Porting

### File Operations
- **Save All** — save every modified open file at once
- **Reload File** — re-read the file from disk, discarding unsaved changes
- **Close All Except Active**
- **Close All to Left / Close All to Right**

### EOL Conversion
Convert line endings between Windows (CRLF), Unix (LF), and Classic Mac (CR):
- Convert to Windows Format
- Convert to Unix Format
- Convert to Mac Format

### Encoding
- Detect and display current file encoding in the status bar
- Convert between encodings (UTF-8, Latin-1, etc.)

### View Options
- **Toggle Whitespace** (already partially present, verify completeness)
- **Toggle EOL Characters**
- **Toggle Indent Guides**
- **Toggle Word Wrap**
- **Full Screen** mode

### Line Operations (Edit menu)
- Move Line Up / Move Line Down
- Duplicate Line (see Scintilla section above)
- Join Lines / Split Lines
- Toggle Line Comment / Toggle Block Comment
- Increase / Decrease Indent

### Remove Operations (Edit menu)
- Remove Empty Lines
- Remove Duplicate Lines
- Remove Consecutive Duplicate Lines
- Reverse Line Order

### Bookmarks
Toggle / clear / navigate bookmarks on individual lines. NotepadNext supports multiple mark
styles and operations like "copy bookmarked lines" and "remove bookmarked lines".

### Find in Files
Search across multiple files in a directory, with results shown in a pane. This is a significant
feature requiring new UI work.

### Multi-selection / Column Mode
Select-all occurrences of the current selection, column/rectangular selection editing.

### Export
- Export as HTML (syntax-colored)
- Print (via system print dialog or text export)
