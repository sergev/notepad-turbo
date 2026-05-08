/*
 * This file is part of Notepad Turbo.
 * Copyright 2026 Serge Vakulenko
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#define Uses_TDialog
#define Uses_TListBox
#define Uses_TButton
#define Uses_TLabel
#define Uses_TScrollBar
#define Uses_TStaticText
#define Uses_TRect
#define Uses_TProgram
#define Uses_TDeskTop
#define Uses_TObject
#define Uses_TCollection
#define Uses_TStringCollection
#include <tvision/tv.h>

#include "NNEncodingDialog.h"

#include <cstring>

namespace {

// All charsets detectable by uchardet, alphabetically sorted.
// These are also the names accepted by iconv for re-encoding on save.
const char * const encodings[] = {
    "BIG5",
    "EUC-JP",
    "EUC-KR",
    "EUC-TW",
    "GB18030",
    "HZ-GB-2312",
    "IBM852",
    "IBM855",
    "IBM865",
    "IBM866",
    "ISO-2022-CN",
    "ISO-2022-JP",
    "ISO-2022-KR",
    "ISO-8859-1",
    "ISO-8859-2",
    "ISO-8859-3",
    "ISO-8859-4",
    "ISO-8859-5",
    "ISO-8859-6",
    "ISO-8859-7",
    "ISO-8859-8",
    "ISO-8859-9",
    "ISO-8859-10",
    "ISO-8859-11",
    "ISO-8859-13",
    "ISO-8859-15",
    "ISO-8859-16",
    "KOI8-R",
    "MAC-CENTRALEUROPE",
    "MAC-CYRILLIC",
    "SHIFT_JIS",
    "TIS-620",
    "UHC",
    "UTF-16BE",
    "UTF-16LE",
    "UTF-8",
    "VISCII",
    "WINDOWS-1250",
    "WINDOWS-1251",
    "WINDOWS-1252",
    "WINDOWS-1253",
    "WINDOWS-1255",
    "WINDOWS-1256",
    "WINDOWS-1257",
    "WINDOWS-1258",
};

constexpr int numEncodings = (int)(sizeof(encodings) / sizeof(encodings[0]));

int findEncoding(const std::string &charset)
{
    for (int i = 0; i < numEncodings; ++i)
        if (charset == encodings[i])
            return i;
    // Default to UTF-8
    for (int i = 0; i < numEncodings; ++i)
        if (std::strcmp(encodings[i], "UTF-8") == 0)
            return i;
    return 0;
}

} // namespace

NNEncodingDialog::NNEncodingDialog(const std::string &currentCharset)
    : currentCharset(currentCharset)
{
}

std::string NNEncodingDialog::run()
{
    // Dialog: 60 wide × 22 tall, centered
    TDialog *d = new TDialog(TRect(0, 0, 60, 22), "File Encoding");
    d->options |= ofCentered;

    std::string label = "Current: " + currentCharset;
    d->insert(new TStaticText(TRect(2, 2, 58, 3), label.c_str()));

    TScrollBar *sb = new TScrollBar(TRect(56, 3, 57, 18));
    d->insert(sb);

    // Two-column list box, 15 rows visible — shows 30 of 43 items at once
    TListBox *lb = new TListBox(TRect(2, 3, 56, 18), 2, sb);
    d->insert(lb);

    d->insert(new TButton(TRect(12, 19, 23, 21), "O~K~",   cmOK,     bfDefault));
    d->insert(new TButton(TRect(35, 19, 46, 21), "Cancel", cmCancel, bfNormal));

    TView *p = TProgram::application->validView(d);
    if (!p) return {};

    // Populate list and pre-select current encoding.
    // Do NOT use TGroup::setData/getData: TListBox expects TListBoxRec (10 bytes
    // on 64-bit), not a bare ushort, so passing &sel would corrupt the list.
    TStringCollection *col = new TStringCollection(numEncodings + 1, 4);
    for (int i = 0; i < numEncodings; ++i)
        col->insert(newStr(encodings[i]));
    lb->newList(col);
    lb->focusItem((short)findEncoding(currentCharset));

    ushort result = TProgram::deskTop->execView(p);
    // Read focused index before destroying the dialog (lb becomes invalid after).
    ushort sel = (result != cmCancel) ? (ushort)lb->focused : (ushort)numEncodings;
    TObject::destroy(p);

    if (sel >= (ushort)numEncodings)
        return {};
    return encodings[sel];
}
