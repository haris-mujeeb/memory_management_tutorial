#include "gtest/gtest.h"
#include "exercise.h"
#include <string.h>

// Assuming TextBuffer structure and smart_append function are defined in exercise.h

TEST(SmartAppendTest, Returns1ForNullValue) {
  TextBuffer dest;
  // Initialize dest (best practice)
  dest.buffer[0] = '\0';
  dest.length = 0;

  const char* src = NULL;
  int result = smart_append(&dest, src);

  // Checks the return value for a null source string
  ASSERT_EQ(1, result);
}

TEST(SmartAppendTest, SmartAppendEmptyBuffer) {
  TextBuffer dest;
  strcpy(dest.buffer, "");
  dest.length = 0;

  const char* src = "Hello";
  int result = smart_append(&dest, src);

  // Checks successful append return value (0)
  ASSERT_EQ(0, result);
  // Checks the content of the destination buffer
  ASSERT_STREQ("Hello", dest.buffer);
  // Checks the updated length
  ASSERT_EQ(5, dest.length);
}

TEST(SmartAppendTest, SmartAppendFullBuffer) {
  TextBuffer dest;
  // Fills the buffer completely (assuming buffer size 64, length 63)
  strcpy(dest.buffer, "This is a very long string that will fill up the entire buffer.");
  dest.length = 63;
  const char* src = " Extra"; // String that cannot fit
  int result = smart_append(&dest, src);

  // Checks unsuccessful append return value (1)
  ASSERT_EQ(1, result);
  // Checks that the buffer content remains unchanged
  ASSERT_STREQ("This is a very long string that will fill up the entire buffer.", dest.buffer);
  // Checks that the length remains unchanged
  ASSERT_EQ(63, dest.length);
}

TEST(SmartAppendTest, SmartAppendOverflow) {
  TextBuffer dest;
  strcpy(dest.buffer, "This is a long string");
  dest.length = 21; // Current length
  const char* src = " that will fill the whole buffer and leave no space for some of the chars.";
  int result = smart_append(&dest, src);

  // Checks overflow return value (1)
  ASSERT_EQ(1, result);
  // Expected final string (length 63): The function should stop writing at the last available byte (index 62)
  ASSERT_STREQ("This is a long string that will fill the whole buffer and leave", dest.buffer);
  // Checks that the length is maxed out
  ASSERT_EQ(63, dest.length);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}