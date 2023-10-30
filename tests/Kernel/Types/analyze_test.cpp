#include "a_lolly_test.hpp"
#include "analyze.hpp"

string
encode_as_utf8 (unsigned int code) {
  // 0x1110dddd 0x10dddddd 0x10dddddd only for simplification
  string str (3);
  str[0]= ((code >> 12) & 0x0F) | 0xE0;
  str[1]= ((code >> 6) & 0x3F) | 0x80;
  str[2]= (code & 0x3F) | 0x80;
  return str;
}
string
cork_to_utf8_stub (string input) {
  int    start= 0, i, n= N (input);
  string r;
  for (i= 0; i < n; i++)
    if (input[i] == '<' && i + 1 < n && input[i + 1] == '#') {
      start= i= i + 2;
      while (i < n && input[i] != '>')
        i++;
      r << encode_as_utf8 (from_hexadecimal (input (start, i)));
      start= i + 1;
    }
  return r;
}

TEST_CASE ("test is alpha") {
  for (unsigned char c= 0; c < 255; c++) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
      CHECK (is_alpha (c));
    }
    else {
      CHECK (!is_alpha (c));
    }
  }
}

TEST_CASE ("cjk_unified_ideographs") {
  CHECK (is_cjk_unified_ideographs ("<#4E2D>"));
  CHECK (has_cjk_unified_ideographs ("<#4E2D>"));
  CHECK (has_cjk_unified_ideographs ("bib-<#4E2D>"));
  CHECK (!is_cjk_unified_ideographs ("bib-<#4E2D>"));
}

TEST_CASE ("test locase all") {
  CHECK_EQ (locase_all (string ("true")) == string ("true"), true);
  CHECK_EQ (locase_all (string ("TRue")) == string ("true"), true);
  CHECK_EQ (locase_all (string ("TRUE")) == string ("true"), true);
  CHECK_EQ (locase_all (string ("123TRUE")) == string ("123true"), true);
}

TEST_CASE ("test upcase all") {
  CHECK_EQ (upcase_all (string ("true")) == string ("TRUE"), true);
  CHECK_EQ (upcase_all (string ("TRue")) == string ("TRUE"), true);
  CHECK_EQ (upcase_all (string ("TRUE")) == string ("TRUE"), true);
  CHECK_EQ (upcase_all (string ("123true")) == string ("123TRUE"), true);
}

TEST_CASE ("test string minus") {
  CHECK_EQ (string_minus ("Hello World", "eo") == string ("Hll Wrld"), true);
  CHECK_EQ (string_minus ("", "abc") == string (""), true);
  CHECK_EQ (string_minus ("abc", "") == string ("abc"), true);
}

TEST_CASE ("test string union") {
  CHECK_EQ (string_union ("abc", "") == string ("abc"), true);
  CHECK_EQ (string_union ("", "abc") == string ("abc"), true);
  CHECK_EQ (string_union ("Hello World", "eo") == string ("Hll Wrldeo"), true);
}

TEST_CASE ("test scm quote") {
  CHECK_EQ (scm_quote ("a") == "\"a\"", true);
  CHECK_EQ (scm_quote ("") == "\"\"", true);
  CHECK_EQ (scm_quote ("\\") == "\"\\\\\"", true);
}

TEST_CASE ("test_scm_unquote") {
  CHECK_EQ (scm_unquote ("\"\"") == "", true);
  CHECK_EQ (scm_unquote ("\"abc\"") == "abc", true);
  CHECK_EQ (scm_unquote ("abc") == "abc", true);
  CHECK_EQ (scm_unquote ("") == "", true);
  CHECK_EQ (scm_unquote ("\"\\\\\"") == "\\", true);
}

TEST_CASE ("test_raw_quote") {
  CHECK_EQ (raw_quote ("a") == "\"a\"", true);
  CHECK_EQ (raw_quote ("") == "\"\"", true);
}

TEST_CASE ("test_raw_unquote") {
  CHECK_EQ (raw_unquote ("\"a\"") == "a", true);
  CHECK_EQ (raw_unquote ("\"a") == "\"a", true);
  CHECK_EQ (raw_unquote ("a\"") == "a\"", true);
  CHECK_EQ (raw_unquote ("") == "", true);
  CHECK_EQ (raw_unquote ("a") == "a", true);
}

TEST_CASE ("test_unescape_guile") {
  CHECK_EQ (unescape_guile ("\\\\") == "\\\\\\\\", true);
}

TEST_CASE ("test_starts") {
  CHECK (starts ("abc_def", "abc"));
  CHECK (!starts ("abc_def", "def"));
  CHECK (starts ("abc", ""));
  CHECK (starts ("", ""));
}

TEST_CASE ("test_ends") {
  CHECK (ends ("abc_def", "def"));
  CHECK (ends ("abc_def", ""));
  CHECK (!ends ("abc_def", "de"));
}

TEST_CASE ("test_read_word") {
  string word;
  int    i= 0;
  CHECK (read_word ("hello123", i, word));
  CHECK_EQ (word == "hello", true);
  CHECK_EQ (i, 5);

  i   = 0;
  word= "";
  CHECK (!read_word ("123", i, word));
  CHECK (is_empty (word));
  CHECK_EQ (i, 0);
}

TEST_CASE ("test_is_digit") {
  for (unsigned char c= 0; c < 255; c++) {
    if (c >= 48 && c <= 57) {
      CHECK (is_digit (c));
    }
    else {
      CHECK (!is_digit (c));
    }
  }
}

TEST_CASE ("test_is_space") {
  for (unsigned char c= 0; c < 255; c++) {
    if ((c == 9) || (c == 10) || (c == 13) || (c == 32)) {
      CHECK (is_space (c));
    }
    else {
      CHECK (!is_space (c));
    }
  }
}

TEST_CASE ("test_is_binary_digit") {
  for (unsigned char c= 0; c < 255; c++) {
    if ((c == '0') || (c == '1')) {
      CHECK (is_binary_digit (c));
    }
    else {
      CHECK (!is_binary_digit (c));
    }
  }
}

TEST_CASE ("replace") {
  CHECK_EQ (replace ("a-b", "-", "_") == "a_b", true);
  CHECK_EQ (replace ("a-b-c", "-", "_") == "a_b_c", true);
}

TEST_CASE ("hanzi_nr") {
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (-1605)) == "负一千六百零五", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (0)) == "零", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1)) == "一", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (10)) == "十", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (11)) == "十一", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (42)) == "四十二", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (90)) == "九十", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (100)) == "一百", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (102)) == "一百零二", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (110)) == "一百一十", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (123)) == "一百二十三", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1000)) == "一千", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1001)) == "一千零一", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1024)) == "一千零二十四", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1030)) == "一千零三十", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1600)) == "一千六百", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (1605)) == "一千六百零五", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (10000)) == "一万", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (10001)) == "一万零一", true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (153457)) == "十五万三千四百五十七",
            true);
  CHECK_EQ (cork_to_utf8_stub (hanzi_nr (300153457)) ==
                "三亿零一十五万三千四百五十七",
            true);
}
