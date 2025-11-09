#include "gtest/gtest.h"
#include "exercise.h"
#include <string.h>

// Group all tests for the format_object function under the Test Suite "FormatObjectTest"

TEST(FormatObjectTest, FormatsInt1) {
  char buffer[100];
  // Assuming new_integer creates and returns a snek_object_t
  snek_object_t i = new_integer(5);
  format_object(i, buffer);

  // Munit: assert_string_equal("int:5", buffer, "formats INTEGER");
  // GTest: ASSERT_STREQ(expected, actual)
  ASSERT_STREQ("int:5", buffer);
}

TEST(FormatObjectTest, FormatsString1) {
  char buffer[100];
  // Assuming new_string creates and returns a snek_object_t
  snek_object_t s = new_string("Hello!");
  format_object(s, buffer);

  // Munit: assert_string_equal("string:Hello!", buffer, "formats STRING");
  // GTest: ASSERT_STREQ(expected, actual)
  ASSERT_STREQ("string:Hello!", buffer);
}

TEST(FormatObjectTest, FormatsInt2) {
  char buffer[100];
  snek_object_t i = new_integer(2014);
  format_object(i, buffer);

  // Munit: assert_string_equal("int:2014", buffer, "formats INTEGER");
  // GTest: ASSERT_STREQ(expected, actual)
  ASSERT_STREQ("int:2014", buffer);
}

TEST(FormatObjectTest, FormatsString2) {
  char buffer[100];
  snek_object_t s = new_string("nvim btw");
  format_object(s, buffer);

  // Munit: assert_string_equal("string:nvim btw", buffer, "formats STRING");
  // GTest: ASSERT_STREQ(expected, actual)
  ASSERT_STREQ("string:nvim btw", buffer);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}