//
// Utility routines for unit tests.
//
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <gtest/gtest.h>

#include <string>

//
// Get current test name, as specified in TEST() macro.
//
std::string get_test_name();

#endif // TEST_UTIL_H
