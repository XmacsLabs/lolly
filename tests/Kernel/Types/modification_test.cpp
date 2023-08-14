#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "modification.hpp"

TEST_CASE ("test construct func") {
    modification m1 (MOD_ASSIGN, path (), tree ());
    modification m2 (MOD_ASSIGN, path (), tree ());
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_ASSIGN, true);
    CHECK_EQ (m1->p == path (), true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_ASSIGN, true);
    CHECK_EQ (m2->p == path (), true);
    CHECK_EQ (m2->t == tree (), true);
}

TEST_CASE ("test mod_assign") {
    modification m1 = mod_assign (path (), tree ());
    modification m2 = mod_assign (path (), tree ());
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_ASSIGN, true);
    CHECK_EQ (m1->p == path (), true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_ASSIGN, true);
    CHECK_EQ (m2->p == path (), true);
    CHECK_EQ (m2->t == tree (), true);
}

TEST_CASE ("test mod_insert") {
    modification m1 = mod_insert (path (), 0, tree ());
    modification m2 = mod_insert (path (), 0, tree ());
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_INSERT, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_INSERT, true);
    CHECK_EQ (m2->t == tree (), true);
}

TEST_CASE ("test mod_remove") {
    modification m1 = mod_remove (path (),1,2);
    modification m2 = mod_remove (path (),1,2);
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_REMOVE, true);
    CHECK_EQ (m1->p[0] == 1, true);
    CHECK_EQ (m1->p[1] == 2, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_REMOVE, true);
    CHECK_EQ (m2->t == tree (), true);
}

TEST_CASE ("test mod_split") {
    modification m1 = mod_split (path (), 1, 2);
    modification m2 = mod_split (path (), 1, 2);
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_SPLIT, true);
    CHECK_EQ (m1->p[0] == 1, true);
    CHECK_EQ (m1->p[1] == 2, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_SPLIT, true);
    CHECK_EQ (m2->t == tree (), true);
}

TEST_CASE ("test mod_join") {
    modification m1 = mod_join (path (), 1);
    modification m2 = mod_join (path (), 1);
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_JOIN, true);
    CHECK_EQ (m1->p[0] == 1, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_JOIN, true);
    CHECK_EQ (m2->t == tree (), true);
    modification m3 = mod_join (path (1), 2);
    CHECK_EQ (m3->k == MOD_JOIN, true);
    CHECK_EQ (m3->p[0] == 1, true);
    CHECK_EQ (m3->p[1] == 2, true);
}

TEST_CASE ("test mod_assign_node") {
    modification m1 = mod_assign_node (path (), 1);
    modification m2 = mod_assign_node (path (), 1);
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_ASSIGN_NODE, true);
    CHECK_EQ (m1->p == path (), true);
    CHECK_EQ (m1->t == tree (1), true);
    CHECK_EQ (m2->k == MOD_ASSIGN_NODE, true);
    CHECK_EQ (m2->p == path (), true);
    CHECK_EQ (m2->t == tree (1), true);
}

TEST_CASE ("test mod_insert_node") {
    modification m1 = mod_insert_node (path (), 1, tree ());
    modification m2 = mod_insert_node (path (), 1, tree ());
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_INSERT_NODE, true);
    CHECK_EQ (m1->p[0] == 1, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_INSERT_NODE, true);
    CHECK_EQ (m2->p[0] == 1, true);
    CHECK_EQ (m2->t == tree (), true);
    modification m3 = mod_insert_node (path (), 2, tree ("string"));
    CHECK_EQ (m3->k == MOD_INSERT_NODE, true);
    CHECK_EQ (m3->p[0] == 2, true);
    CHECK_EQ (m3->t == tree ("string"), true);
}

TEST_CASE ("test mod_remove_node") {
    modification m1 = mod_remove_node (path (), 1);
    modification m2 = mod_remove_node (path (), 1);
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_REMOVE_NODE, true);
    CHECK_EQ (m1->p[0] == 1, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_REMOVE_NODE, true);
    CHECK_EQ (m2->p[0] == 1, true);
    CHECK_EQ (m2->t == tree (), true);
}

TEST_CASE ("test mod_set_cursor") {
    modification m1 = mod_set_cursor (path (), 1, tree ());
    modification m2 = mod_set_cursor (path (), 1, tree ());
    CHECK_EQ (m1 == m2, true);
    CHECK_EQ (m1 != m2, false);
    CHECK_EQ (m1->k == MOD_SET_CURSOR, true);
    CHECK_EQ (m1->p[0] == 1, true);
    CHECK_EQ (m1->t == tree (), true);
    CHECK_EQ (m2->k == MOD_SET_CURSOR, true);
    CHECK_EQ (m2->p[0] == 1, true);
    CHECK_EQ (m2->t == tree (), true);
    modification m3 = mod_set_cursor (path (), 2, tree ("string"));
    CHECK_EQ (m3->k == MOD_SET_CURSOR, true);
    CHECK_EQ (m3->p[0] == 2, true);
    CHECK_EQ (m3->t == tree ("string"), true);
}