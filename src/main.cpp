#include "gtest/gtest.h"
#include "exercise.h"
#include <string.h>

TEST(ConcatStrings, EmptyStrings) {
  char str1[100] = "";
  const char *str2 = "";
  concat_strings(str1, str2);
  ASSERT_STREQ(str1, "");
}

TEST(ConcatStrings, EmptyToNonEmpty) {
  char str1[100] = "Hello";
  const char *str2 = "";
  concat_strings(str1, str2);
  ASSERT_STREQ(str1, "Hello");
}

TEST(ConcatStrings, SmallStrings) {
  char str1[100] = "Hello ";
  const char *str2 = "World";
  concat_strings(str1, str2);
  ASSERT_STREQ(str1, "Hello World");
}

TEST(ConcatStrings, LongStrings) {
  char str1[200] = "This is a longer string that ";
  const char *str2 = "will be concatenated with another long string.";
  concat_strings(str1, str2);
  ASSERT_STREQ(str1, "This is a longer string that will be concatenated with another long string.");
}

TEST(ConcatStrings, NonEmptyToEmpty) {
  char str1[100] = "";
  const char *str2 = "Hello";
  concat_strings(str1, str2);
  ASSERT_STREQ(str1, "Hello");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}