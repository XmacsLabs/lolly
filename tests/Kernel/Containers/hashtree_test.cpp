#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#undef FAILED
#undef ERROR
#include "hashtree.hpp"
#include "string.hpp"

// Create a hashtree object with integer keys and string values

TEST_CASE ("Test the add_child function") {
  hashtree<int, string> t;

  string a ("one");
  string b ("two");

  hashtree<int, string> child1 (string ("one"));
  hashtree<int, string> child2 (string ("two"));
  t->add_child (1, child1);
  t->add_child (2, child2);
  CHECK (N (child1) == 0);
}

TEST_CASE ("Test the contains function") {
  hashtree<int, string> t;

  string a ("one");
  string b ("two");

  hashtree<int, string> child1 (string ("one"));
  hashtree<int, string> child2 (string ("two"));
  t->add_child (1, child1);
  t->add_child (2, child2);
  CHECK (t->contains (1));
  CHECK (t->contains (2));
  CHECK (!t->contains (3));
}

TEST_CASE ("Test the add_new_child function") {
  hashtree<int, string> t;

  string a ("one");
  string b ("two");

  hashtree<int, string> child1 (string ("one"));
  hashtree<int, string> child2 (string ("two"));
  t->add_child (1, child1);
  t->add_child (2, child2);
  t->add_new_child (3);
  CHECK_EQ (N (child2) == 3, false);
  CHECK (t->contains (3));
}

TEST_CASE ("Test the set_label and get_label function") {
  hashtree<int, string> t;
  t->set_label ("root");
  CHECK (t->get_label () == "root");
}

TEST_CASE ("Test the N and is_nil functions") {
  hashtree<int, std::string> nil_tree;
  CHECK_EQ (is_nil (nil_tree), false);
}