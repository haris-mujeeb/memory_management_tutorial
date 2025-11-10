#include "gtest/gtest.h"
#include "exercise.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h> // Required for uintptr_t

// --- Helper function from original Munit test ---
// NOTE: This function relies on GCC built-ins and may not be portable.
// It is kept here to maintain the functionality of the original test.
bool is_on_stack(void *ptr) {
  void *stack_top = __builtin_frame_address(0);
  uintptr_t stack_top_addr = (uintptr_t)stack_top;
  uintptr_t ptr_addr = (uintptr_t)ptr;

  // Check within a threshold in both directions (e.g., 1KB)
  uintptr_t threshold = 1024;

  return ptr_addr >= (stack_top_addr - threshold) && ptr_addr <= (stack_top_addr + threshold);
}
// ------------------------------------------------

// Group all tests for get_full_greeting under the Test Suite "GreetingTest"

TEST(GreetingTest, BasicGreeting) {
  // get_full_greeting(prefix, name, buffer_size)
  char *result = get_full_greeting("Hello", "Alice", 20);

  // Checks the expected full greeting
  ASSERT_STREQ("Hello Alice", result);
  
  // Checks that the result buffer was dynamically allocated (not on the stack)
  ASSERT_FALSE(is_on_stack(result));

  // Cleans up the dynamically allocated memory
  free(result);
}

TEST(GreetingTest, ShortBuffer) {
  // get_full_greeting(prefix, name, buffer_size)
  // The full string "Hey Bob" (7 chars + null terminator = 8 needed) won't fit in size 4.
  char *result = get_full_greeting("Hey", "Bob", 4);

  // Checks that the string is truncated to fit the requested buffer size
  ASSERT_STREQ("Hey", result);
  
  // Checks that the result buffer was dynamically allocated
  ASSERT_FALSE(is_on_stack(result));

  // Cleans up the dynamically allocated memory
  free(result);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}