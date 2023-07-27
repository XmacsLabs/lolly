#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#undef FAILED
#undef ERROR
#include "iterator.hpp"
using namespace lolly;

hashset<int> set;

TEST_CASE ("Test iterator_rep") {
  set->insert (1);
  set->insert (2);
  SUBCASE ("Test iterator busy") {
    iterator<int> it= iterate (set);
    CHECK (it->busy ());
  }
  SUBCASE ("Test iterator remains") {
    iterator<int> it= iterate (set);
    CHECK (it->remains () == -1);
  }
  SUBCASE ("Test iterator next") {
    iterator<int> it= iterate (set);
    CHECK (it->next ());
  }
}

TEST_CASE ("Test iterate with hashmap") {
  hashmap<int, int> h;
  h (1)                = 1;
  h (2)                = 4;
  h (3)                = 6;
  iterator<int>      it= iterate (h);
  std::ostringstream oss;
  while (it->busy ()) {
    oss << it->next () << " ";
  }
  CHECK_EQ (oss.str (), "1 2 3 ");
}

TEST_CASE ("Test iterate with hashset") {
  hashset<int> h;
  h->insert (1);
  h->insert (2);
  h->insert (3);
  iterator<int>      it= iterate (h);
  std::ostringstream oss;
  while (it->busy ()) {
    oss << it->next () << " ";
  }
  CHECK_EQ (oss.str (), "1 2 3 ");
}