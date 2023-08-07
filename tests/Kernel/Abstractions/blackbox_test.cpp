/** \file blackbox_test.cpp
 *  \copyright GPLv3
 *  \details A unitest for blackbox.
 *  \author YuanYe
 *  \date   2023
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "blackbox.hpp"
#include "doctest/doctest.h"
#include "string.hpp"

blackbox b0;
blackbox b1 = close_box<SI> (-2147483648); // int
blackbox b2 = close_box<SN> (2147483647);  // unsigned int
blackbox b3 = close_box<HI> (-32768);      // short
blackbox b4 = close_box<HN> (32767);       // unsiened short
blackbox b5 = close_box<QI> ('`');         // char
blackbox b6 = close_box<QN> (255);         // unsigned char
blackbox b7 = close_box<DI> (LLONG_MAX);   // long long
blackbox b8 = close_box<DN> (ULLONG_MAX);  // unsigned long long
blackbox b9 = close_box<pointer> (0);      // void*
blackbox b10= close_box<float> (3.14);
blackbox b11= close_box<double> (3.14159265);
blackbox b12= close_box<long double> (3.1415926535);

blackbox t0;
blackbox t1 = close_box<SI> (0);      // int
blackbox t2 = close_box<SN> (0);      // unsigned int
blackbox t3 = close_box<HI> (0);      // short
blackbox t4 = close_box<HN> (0);      // unsiened short
blackbox t5 = close_box<QI> (0);      // char
blackbox t6 = close_box<QN> (0);      // unsigned char
blackbox t7 = close_box<DI> (0);      // long long
blackbox t8 = close_box<DN> (0);      // unsigned long long
blackbox t9 = close_box<pointer> (0); // void*
blackbox t10= close_box<float> (0);
blackbox t11= close_box<double> (0);
blackbox t12= close_box<long double> (0);

blackbox t[13]   = {t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
blackbox b[13]   = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12};
string   bout[13]= {"",
                    "-2147483648\0",
                    "2147483647",
                    "-32768\0",
                    "32767",
                    "`",
                    "255",
                    "9223372036854775807",
                    "18446744073709551615",
                    "false",
                    "3.14",
                    "3.14159",
                    "3.14159"};
string   tout[13]= {"",
                    "-2147483648\n",
                    "2147483647\t",
                    "-327680",
                    "32767 ",
                    "`\n",
                    "0",
                    "9223372036854775807\n",
                    "18446744073709551615 ",
                    "0",
                    "3.1415926",
                    "3.14159265",
                    "3.1415926535"};

TEST_CASE ("test for get_type()") {
  for (int i= 1; i < 13; i++) {
    CHECK_EQ (b[i]->get_type () == -13 + i, true);
    CHECK_EQ (t[i]->get_type () == -13 + i, true);
  }
}

TEST_CASE ("test for equal(), operator== and operator!=") {
  SUBCASE ("test for operator==") {
    for (int i= 0; i < 13; i++) {
      CHECK_EQ (b[i] == b[i], true);
      if (i == 0 || i == 9) CHECK_EQ (b[i] == t[i], true);
      else CHECK_EQ (b[i] != t[i], true);
      for (int j= 0; j < 13; j++) {
        if (i != j) CHECK_EQ (b[i] != b[j], true);
      }
    }
  }
}

TEST_CASE ("Test fot display() and operator<<") {
  tm_ostream out;
  string     str;
  for (int i= 0; i < 13; i++) {
    out.buffer ();
    out << b[i];
    str= out.unbuffer ();
    CHECK_EQ (str != tout[i], true);
    if (i != 0) CHECK_EQ (str == bout[i], true);
    else CHECK_EQ (str != bout[i], true);
  }
}