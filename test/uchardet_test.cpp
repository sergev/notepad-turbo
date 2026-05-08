#include "uchardet.h"

#include <gtest/gtest.h>

#include <cctype>
#include <cstdio>
#include <string>

static std::string detectFile(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "rb");
    uchardet_t handle = uchardet_new();
    char buf[65536];
    while (size_t n = fread(buf, 1, sizeof(buf), fp))
        uchardet_handle_data(handle, buf, n);
    uchardet_data_end(handle);
    std::string charset = uchardet_get_charset(handle);
    uchardet_delete(handle);
    fclose(fp);
    for (char& c : charset)
        c = static_cast<char>(tolower(c));
    return charset;
}

// ar
TEST(Uchardet, ar_iso_8859_6)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ar/iso-8859-6.txt"), "iso-8859-6");
}
TEST(Uchardet, ar_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ar/utf-8.txt"), "utf-8");
}
TEST(Uchardet, ar_windows_1256)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ar/windows-1256.txt"), "windows-1256");
}

// bg
TEST(Uchardet, bg_windows_1251)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/bg/windows-1251.txt"), "windows-1251");
}

// cs
TEST(Uchardet, cs_ibm852)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/cs/ibm852.txt"), "ibm852");
}
TEST(Uchardet, cs_iso_8859_2)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/cs/iso-8859-2.txt"), "iso-8859-2");
}
TEST(Uchardet, cs_mac_centraleurope)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/cs/mac-centraleurope.txt"), "mac-centraleurope");
}
TEST(Uchardet, cs_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/cs/utf-8.txt"), "utf-8");
}
TEST(Uchardet, cs_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/cs/windows-1250.txt"), "windows-1250");
}

// da
TEST(Uchardet, da_ibm865)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/da/ibm865.txt"), "ibm865");
}
TEST(Uchardet, da_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/da/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, da_iso_8859_15)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/da/iso-8859-15.txt"), "iso-8859-15");
}
TEST(Uchardet, da_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/da/utf-8.txt"), "utf-8");
}
TEST(Uchardet, da_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/da/windows-1252.txt"), "windows-1252");
}

// de
TEST(Uchardet, de_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/de/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, de_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/de/windows-1252.txt"), "windows-1252");
}

// el
TEST(Uchardet, el_iso_8859_7)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/el/iso-8859-7.txt"), "iso-8859-7");
}
TEST(Uchardet, el_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/el/utf-8.txt"), "utf-8");
}
TEST(Uchardet, el_windows_1253)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/el/windows-1253.txt"), "windows-1253");
}

// en
TEST(Uchardet, en_ascii)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/en/ascii.txt"), "ascii");
}

// eo
TEST(Uchardet, eo_iso_8859_3)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/eo/iso-8859-3.txt"), "iso-8859-3");
}

// es
TEST(Uchardet, es_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/es/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, es_iso_8859_15)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/es/iso-8859-15.txt"), "iso-8859-15");
}
TEST(Uchardet, es_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/es/utf-8.txt"), "utf-8");
}
TEST(Uchardet, es_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/es/windows-1252.txt"), "windows-1252");
}

// et
TEST(Uchardet, et_iso_8859_13)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/et/iso-8859-13.txt"), "iso-8859-13");
}
TEST(Uchardet, et_iso_8859_15)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/et/iso-8859-15.txt"), "iso-8859-15");
}
TEST(Uchardet, et_iso_8859_4)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/et/iso-8859-4.txt"), "iso-8859-4");
}
TEST(Uchardet, et_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/et/utf-8.txt"), "utf-8");
}
TEST(Uchardet, et_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/et/windows-1252.txt"), "windows-1252");
}
TEST(Uchardet, et_windows_1257)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/et/windows-1257.txt"), "windows-1257");
}

// fi
TEST(Uchardet, fi_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/fi/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, fi_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/fi/utf-8.txt"), "utf-8");
}

// fr
TEST(Uchardet, fr_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/fr/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, fr_iso_8859_15)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/fr/iso-8859-15.txt"), "iso-8859-15");
}
TEST(Uchardet, fr_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/fr/utf-8.txt"), "utf-8");
}
TEST(Uchardet, fr_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/fr/windows-1252.txt"), "windows-1252");
}

// ga
TEST(Uchardet, ga_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ga/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, ga_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ga/utf-8.txt"), "utf-8");
}
TEST(Uchardet, ga_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ga/windows-1252.txt"), "windows-1252");
}

// he
TEST(Uchardet, he_iso_8859_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/he/iso-8859-8.txt"), "iso-8859-8");
}
TEST(Uchardet, he_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/he/utf-8.txt"), "utf-8");
}
TEST(Uchardet, he_windows_1255)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/he/windows-1255.txt"), "windows-1255");
}

// hr
TEST(Uchardet, hr_ibm852)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/ibm852.txt"), "ibm852");
}
TEST(Uchardet, hr_iso_8859_13)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/iso-8859-13.txt"), "iso-8859-13");
}
TEST(Uchardet, hr_iso_8859_16)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/iso-8859-16.txt"), "iso-8859-16");
}
TEST(Uchardet, hr_iso_8859_2)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/iso-8859-2.txt"), "iso-8859-2");
}
TEST(Uchardet, hr_mac_centraleurope)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/mac-centraleurope.txt"), "mac-centraleurope");
}
TEST(Uchardet, hr_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/utf-8.txt"), "utf-8");
}
TEST(Uchardet, hr_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hr/windows-1250.txt"), "windows-1250");
}

// hu
TEST(Uchardet, hu_iso_8859_2)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hu/iso-8859-2.txt"), "iso-8859-2");
}
TEST(Uchardet, hu_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/hu/windows-1250.txt"), "windows-1250");
}

// it
TEST(Uchardet, it_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/it/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, it_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/it/utf-8.txt"), "utf-8");
}

// ja
TEST(Uchardet, ja_euc_jp)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ja/euc-jp.txt"), "euc-jp");
}
TEST(Uchardet, ja_iso_2022_jp)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ja/iso-2022-jp.txt"), "iso-2022-jp");
}
TEST(Uchardet, ja_shift_jis)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ja/shift_jis.txt"), "shift_jis");
}
TEST(Uchardet, ja_utf_16be)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ja/utf-16be.txt"), "utf-16be");
}
TEST(Uchardet, ja_utf_16le)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ja/utf-16le.txt"), "utf-16le");
}
TEST(Uchardet, ja_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ja/utf-8.txt"), "utf-8");
}

// ko
TEST(Uchardet, ko_iso_2022_kr)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ko/iso-2022-kr.txt"), "iso-2022-kr");
}
TEST(Uchardet, ko_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ko/utf-8.txt"), "utf-8");
}

// lt
TEST(Uchardet, lt_iso_8859_10)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lt/iso-8859-10.txt"), "iso-8859-10");
}
TEST(Uchardet, lt_iso_8859_13)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lt/iso-8859-13.txt"), "iso-8859-13");
}
TEST(Uchardet, lt_iso_8859_4)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lt/iso-8859-4.txt"), "iso-8859-4");
}
TEST(Uchardet, lt_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lt/utf-8.txt"), "utf-8");
}

// lv
TEST(Uchardet, lv_iso_8859_10)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lv/iso-8859-10.txt"), "iso-8859-10");
}
TEST(Uchardet, lv_iso_8859_13)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lv/iso-8859-13.txt"), "iso-8859-13");
}
TEST(Uchardet, lv_iso_8859_4)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lv/iso-8859-4.txt"), "iso-8859-4");
}
TEST(Uchardet, lv_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/lv/utf-8.txt"), "utf-8");
}

// mt
TEST(Uchardet, mt_iso_8859_3)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/mt/iso-8859-3.txt"), "iso-8859-3");
}
TEST(Uchardet, mt_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/mt/utf-8.txt"), "utf-8");
}

// no
TEST(Uchardet, no_ibm865)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/no/ibm865.txt"), "ibm865");
}
TEST(Uchardet, no_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/no/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, no_iso_8859_15)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/no/iso-8859-15.txt"), "iso-8859-15");
}
TEST(Uchardet, no_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/no/utf-8.txt"), "utf-8");
}
TEST(Uchardet, no_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/no/windows-1252.txt"), "windows-1252");
}

// pl
TEST(Uchardet, pl_ibm852)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/ibm852.txt"), "ibm852");
}
TEST(Uchardet, pl_iso_8859_13)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/iso-8859-13.txt"), "iso-8859-13");
}
TEST(Uchardet, pl_iso_8859_16)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/iso-8859-16.txt"), "iso-8859-16");
}
TEST(Uchardet, pl_iso_8859_2)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/iso-8859-2.txt"), "iso-8859-2");
}
TEST(Uchardet, pl_mac_centraleurope)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/mac-centraleurope.txt"), "mac-centraleurope");
}
TEST(Uchardet, pl_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/utf-8.txt"), "utf-8");
}
TEST(Uchardet, pl_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pl/windows-1250.txt"), "windows-1250");
}

// pt
TEST(Uchardet, pt_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pt/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, pt_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/pt/utf-8.txt"), "utf-8");
}

// ro
TEST(Uchardet, ro_ibm852)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ro/ibm852.txt"), "ibm852");
}
TEST(Uchardet, ro_iso_8859_16)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ro/iso-8859-16.txt"), "iso-8859-16");
}
TEST(Uchardet, ro_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ro/utf-8.txt"), "utf-8");
}
TEST(Uchardet, ro_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ro/windows-1250.txt"), "windows-1250");
}

// ru
TEST(Uchardet, ru_ibm855)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ru/ibm855.txt"), "ibm855");
}
TEST(Uchardet, ru_ibm866)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ru/ibm866.txt"), "ibm866");
}
TEST(Uchardet, ru_iso_8859_5)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ru/iso-8859-5.txt"), "iso-8859-5");
}
TEST(Uchardet, ru_koi8_r)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ru/koi8-r.txt"), "koi8-r");
}
TEST(Uchardet, ru_mac_cyrillic)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ru/mac-cyrillic.txt"), "mac-cyrillic");
}
TEST(Uchardet, ru_windows_1251)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/ru/windows-1251.txt"), "windows-1251");
}

// sk
TEST(Uchardet, sk_ibm852)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sk/ibm852.txt"), "ibm852");
}
TEST(Uchardet, sk_iso_8859_2)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sk/iso-8859-2.txt"), "iso-8859-2");
}
TEST(Uchardet, sk_mac_centraleurope)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sk/mac-centraleurope.txt"), "mac-centraleurope");
}
TEST(Uchardet, sk_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sk/utf-8.txt"), "utf-8");
}
TEST(Uchardet, sk_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sk/windows-1250.txt"), "windows-1250");
}

// sl
TEST(Uchardet, sl_ibm852)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sl/ibm852.txt"), "ibm852");
}
TEST(Uchardet, sl_iso_8859_16)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sl/iso-8859-16.txt"), "iso-8859-16");
}
TEST(Uchardet, sl_iso_8859_2)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sl/iso-8859-2.txt"), "iso-8859-2");
}
TEST(Uchardet, sl_mac_centraleurope)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sl/mac-centraleurope.txt"), "mac-centraleurope");
}
TEST(Uchardet, sl_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sl/utf-8.txt"), "utf-8");
}
TEST(Uchardet, sl_windows_1250)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sl/windows-1250.txt"), "windows-1250");
}

// sv
TEST(Uchardet, sv_iso_8859_1)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sv/iso-8859-1.txt"), "iso-8859-1");
}
TEST(Uchardet, sv_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sv/utf-8.txt"), "utf-8");
}
TEST(Uchardet, sv_windows_1252)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/sv/windows-1252.txt"), "windows-1252");
}

// th
TEST(Uchardet, th_iso_8859_11)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/th/iso-8859-11.txt"), "iso-8859-11");
}
TEST(Uchardet, th_tis_620)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/th/tis-620.txt"), "tis-620");
}
TEST(Uchardet, th_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/th/utf-8.txt"), "utf-8");
}

// tr
TEST(Uchardet, tr_iso_8859_3)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/tr/iso-8859-3.txt"), "iso-8859-3");
}
TEST(Uchardet, tr_iso_8859_9)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/tr/iso-8859-9.txt"), "iso-8859-9");
}

// vi
TEST(Uchardet, vi_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/vi/utf-8.txt"), "utf-8");
}
TEST(Uchardet, vi_viscii)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/vi/viscii.txt"), "viscii");
}
TEST(Uchardet, vi_windows_1258)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/vi/windows-1258.txt"), "windows-1258");
}

// zh
TEST(Uchardet, zh_big5)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/zh/big5.txt"), "big5");
}
TEST(Uchardet, zh_euc_tw)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/zh/euc-tw.txt"), "euc-tw");
}
TEST(Uchardet, zh_gb18030)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/zh/gb18030.txt"), "gb18030");
}
TEST(Uchardet, zh_utf_8)
{
    EXPECT_EQ(detectFile(std::string(UCHARDET_TEST_DIR) + "/zh/utf-8.txt"), "utf-8");
}
