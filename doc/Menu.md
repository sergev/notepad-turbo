# Menu Structure

## NotepadNext Reference Menu

NotepadNext is a Qt-based GUI application. Its menu structure is defined in
`src/dialogs/MainWindow.ui` and populated dynamically in `MainWindow.cpp`.

### File

| Item | Shortcut |
|------|----------|
| New | Ctrl+N |
| Open... | Ctrl+O |
| Open Folder as Workspace... | |
| Reload | |
| Save | Ctrl+S |
| Save As... | Ctrl+Alt+S |
| Save a Copy As... | |
| Save All | Ctrl+Shift+S |
| Export As > Export as HTML... | |
| Export As > Export as RTF... | |
| Rename... | |
| Close | Ctrl+W |
| Close All | Ctrl+Shift+W |
| Close More > Close All Except Active | |
| Close More > Close All to the Left | |
| Close More > Close All to the Right | |
| Move to Trash... | |
| Print... | Ctrl+P |
| Recent Files > Restore Recently Closed | Ctrl+Shift+T |
| Recent Files > Open All Recent Files | |
| Recent Files > Clear Recent Files List | |
| Recent Files > \[dynamic list\] | |
| Exit | |

### Edit

| Item | Shortcut |
|------|----------|
| Undo | Ctrl+Z |
| Redo | Ctrl+Y |
| Cut | Ctrl+X |
| Copy | Ctrl+C |
| Paste | Ctrl+V |
| Delete | Del |
| Select All | Ctrl+A |
| Select Next Instance | Ctrl+D |
| Copy More > Copy Full Path | |
| Copy More > Copy File Name | |
| Copy More > Copy File Directory | |
| Copy As > Copy as HTML | |
| Copy As > Copy as RTF | |
| Indent > Increase Indent | |
| Indent > Decrease Indent | |
| Convert Case > UPPER CASE | |
| Convert Case > lower case | |
| EOL Conversion > Windows (CR LF) | |
| EOL Conversion > Unix (LF) | |
| EOL Conversion > Macintosh (CR) | |
| Line Operations > Duplicate Current Line | Alt+Down |
| Line Operations > Split Lines | |
| Line Operations > Join Lines | Ctrl+J |
| Line Operations > Move Lines Up | Ctrl+Shift+Up |
| Line Operations > Move Lines Down | Ctrl+Shift+Down |
| Line Operations > Remove Empty Lines | |
| Line Operations > Remove Duplicate Lines | |
| Line Operations > Remove Consecutive Duplicates | |
| Line Operations > Reverse Line Order | |
| Line Operations > Sort Ascending | |
| Line Operations > Sort Ascending (Case-Insensitive) | |
| Line Operations > Sort by Length Ascending | |
| Line Operations > Sort Descending | |
| Line Operations > Sort Descending (Case-Insensitive) | |
| Line Operations > Sort by Length Descending | |
| Comment/Uncomment > Toggle Single Line Comment | Ctrl+/ |
| Comment/Uncomment > Single Line Comment | Ctrl+K |
| Comment/Uncomment > Single Line Uncomment | Ctrl+Shift+K |
| Encoding/Decoding > Base64 Encode | |
| Encoding/Decoding > URL Encode | |
| Encoding/Decoding > Base64 Decode | |
| Encoding/Decoding > URL Decode | |
| Column Mode... | |

### Search

| Item | Shortcut |
|------|----------|
| Find... | Ctrl+F |
| Find in Files... | |
| Find Next | F3 |
| Find Previous | Shift+F3 |
| Replace... | Ctrl+H |
| Quick Find | Ctrl+Alt+I |
| Go to Line... | Ctrl+G |
| Mark All Occurrences > Style 1–3 | |
| Clear Marks > Style 1–3, Clear All | |
| Bookmarks > Toggle Bookmark | Ctrl+F2 |
| Bookmarks > Next Bookmark | F2 |
| Bookmarks > Previous Bookmark | Shift+F2 |
| Bookmarks > Clear Bookmarks | |
| Bookmarks > Invert Bookmarks | |
| Bookmarks > Cut/Copy/Delete Bookmarked Lines | |

### View

| Item | Shortcut |
|------|----------|
| Full Screen | F11 |
| Show Symbol > Show All Characters | |
| Show Symbol > Show Whitespace | |
| Show Symbol > Show End of Line | |
| Show Symbol > Show Indent Guide | |
| Show Symbol > Show Wrap Symbol | |
| Zoom > Zoom In | Ctrl++ |
| Zoom > Zoom Out | Ctrl+- |
| Zoom > Reset Zoom | Ctrl+0 |
| Word Wrap | |
| Fold All | Alt+0 |
| Unfold All | Alt+Shift+0 |
| Fold Level 1–9 | Alt+1–9 |
| Unfold Level 1–9 | Alt+Shift+1–9 |

### Language

Dynamically populated with all supported languages, organized alphabetically.

### Settings

| Item | Shortcut |
|------|----------|
| Preferences... | |

### Macro

| Item | Shortcut |
|------|----------|
| Start Recording | |
| Playback | Ctrl+Shift+P |
| Run a Macro Multiple Times... | |
| Save Current Recorded Macro... | |
| Edit Macros... | |
| \[dynamic saved macros\] | |

### Help

| Item | Shortcut |
|------|----------|
| Check for Updates... | |
| About Qt | |
| About Notepad Next | |
| Debug Info... | |

---

## Current Notepad Turbo Menu

Defined in [src/NNMenus.cpp](../src/NNMenus.cpp), commands in [src/NNCommands.h](../src/NNCommands.h).

### File (Alt-F)

| Item | Shortcut |
|------|----------|
| New | Ctrl-N |
| Open... | F3 |
| Save | F2 |
| Save As... | |
| Close | Ctrl-W |
| Close All | |
| Exit | Ctrl-Q |

### Edit (Alt-E)

| Item | Shortcut |
|------|----------|
| Undo | Ctrl-Z |
| Cut | Ctrl-X |
| Copy | Ctrl-C |
| Paste | Ctrl-V |
| Delete | Del |
| Select All | Ctrl-A |

### Search (Alt-S)

| Item | Shortcut |
|------|----------|
| Find... | Ctrl-F |
| Replace... | Ctrl-H |
| Find Next | F4 |
| Find Previous | Shift-F4 |
| Goto Line... | Ctrl-G |

### View (Alt-V)

| Item | Shortcut |
|------|----------|
| Word Wrap | |
| Whitespace | |

### Tools (Alt-T)

| Item | Shortcut |
|------|----------|
| Preferences... | |
| Record Macro | |
| Stop Recording | |
| Run Macro | |
| Save Macro | |

### Window (Alt-W)

| Item | Shortcut |
|------|----------|
| Tile | |
| Cascade | |
| Next | F6 |
| Previous | Shift-F6 |

### Help (Alt-H)

| Item | Shortcut |
|------|----------|
| About... | |

---

## Proposed Menu for Notepad Turbo

The Turbo Vision TUI framework constrains the UX: no infinite submenus, limited
key bindings, and a text-mode display. The proposed structure below adapts
NotepadNext's feature set to TUI conventions while aligning with what is already
implemented or planned (see `NNCommands.h`).

### File (Alt-F)

| Item | Shortcut | Notes |
|------|----------|-------|
| New | Ctrl-N | |
| Open... | F3 | |
| Reload | | Reopen current file from disk |
| ─── | | |
| Save | F2 | |
| Save As... | | |
| Save All | Ctrl-S | |
| ─── | | |
| Close | Ctrl-W | |
| Close All | | |
| ─── | | |
| Recent Files ▶ | | Submenu with up to 10 paths |
| ─── | | |
| Exit | Ctrl-Q | |

### Edit (Alt-E)

| Item | Shortcut | Notes |
|------|----------|-------|
| Undo | Ctrl-Z | |
| Redo | Ctrl-Y | |
| ─── | | |
| Cut | Ctrl-X | |
| Copy | Ctrl-C | |
| Paste | Ctrl-V | |
| Delete | Del | |
| ─── | | |
| Select All | Ctrl-A | |
| ─── | | |
| Convert Case ▶ | | UPPER / lower / Title |
| EOL Conversion ▶ | | Windows / Unix / Mac |
| Line Operations ▶ | | Duplicate, Move Up/Down, Sort |
| Comment/Uncomment | Ctrl-/ | Language-aware via Lua |

### Search (Alt-S)

| Item | Shortcut | Notes |
|------|----------|-------|
| Find... | Ctrl-F | |
| Replace... | Ctrl-H | |
| Find Next | F4 | |
| Find Previous | Shift-F4 | |
| ─── | | |
| Goto Line... | Ctrl-G | |

### View (Alt-V)

| Item | Shortcut | Notes |
|------|----------|-------|
| Word Wrap | | Toggle checkmark |
| Whitespace | | Toggle checkmark |
| Line Numbers | | Toggle checkmark |
| ─── | | |
| Zoom In | Ctrl-+ | |
| Zoom Out | Ctrl-- | |
| Reset Zoom | Ctrl-0 | |

### Language (Alt-L)

| Item | Shortcut | Notes |
|------|----------|-------|
| \[language list\] | | Dynamically populated from Lua scripts, grouped A–Z |

### Tools (Alt-T)

| Item | Shortcut | Notes |
|------|----------|-------|
| Preferences... | | |
| ─── | | |
| Start Recording | | Toggle; grayed when playing back |
| Stop Recording | | |
| Run Macro | Ctrl-Shift-P | |
| Save Macro... | | |

### Window (Alt-W)

| Item | Shortcut | Notes |
|------|----------|-------|
| Tile | | |
| Cascade | | |
| ─── | | |
| Next | F6 | |
| Previous | Shift-F6 | |

### Help (Alt-H)

| Item | Shortcut | Notes |
|------|----------|-------|
| About... | | Version + build info |

---

### Design Notes

- **Language menu** is promoted to a top-level item (from a submenu inside
  View/Settings) to match NotepadNext and make language switching fast.
- **Redo** is added to Edit; it exists in `NNCommands.h` placeholder but is not
  yet in the menu.
- **Save All** replaces the bare `Ctrl-S` binding; individual Save stays on F2.
- **Line Operations** and **Convert Case** submenus add text-manipulation
  features available in NotepadNext without cluttering the top level.
- **EOL Conversion** is useful for cross-platform editing; trivial to add once
  the underlying document API supports it.
- **Zoom** belongs in View rather than being hidden; Turbo Vision supports font
  scaling in some terminal configurations.
- **Recent Files** submenu is already backed by `RecentFilesListManager`.
- Items that require significant new infrastructure (bookmarks, find-in-files,
  column mode, folding) are intentionally omitted until the core is ready.
