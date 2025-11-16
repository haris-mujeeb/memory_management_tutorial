#include "gtest/gtest.h"
#include "exercise.h"
#include "snekstack.h"
#include <stdlib.h>
#include <stdint.h> // For intptr_t

// Group all stack tests under the "SnekStackTest" suite

TEST(SnekStackTest, HeterogenousStack) {
  snek_stack_t *s = stack_new(2);
  ASSERT_NE(nullptr, s) << "Must allocate a new stack";

  // This function presumably pushes a raw integer value and a pointer to an integer
  scary_double_push(s);
  ASSERT_EQ(2, s->count) << "Should have two items in the stack";

  // Test 1: The first item is a raw integer value (1337) stored as a void*
  // We must cast the void* to an integer type (intptr_t) to check its value.
  
  intptr_t value = reinterpret_cast<intptr_t>(s->data[0]);
  ASSERT_EQ(1337, value) << "Zero item should be 1337";

  // Test 2: The second item is a pointer to an integer (1024)
  int *pointer = (int*)s->data[1];
  ASSERT_NE(nullptr, pointer) << "Pointer on stack should not be null";
  ASSERT_EQ(1024, *pointer) << "Top item should be 1024";

  // Cleanup
  free(pointer); // Free the memory pointed to by the stack item
  stack_free(s); // Free the stack itself
}

TEST(SnekStackTest, MultipleTypesStack) {
  snek_stack_t *s = stack_new(4);
  ASSERT_NE(nullptr, s) << "Must allocate a new stack";

  // This function pushes pointers to different data types
  stack_push_multiple_types(s);
  ASSERT_EQ(2, s->count) << "Should have two items in the stack";

  // Test 1: Check the float pointer
  float *f = (float*)s->data[0];
  ASSERT_NE(nullptr, f) << "Float pointer should not be null";
  // Munit: assert_float(*f, ==, 3.14, ...)
  // GTest: ASSERT_FLOAT_EQ(expected, actual)
  ASSERT_FLOAT_EQ(3.14, *f) << "Float is equal";

  // Test 2: Check the char pointer (string)
  char *string = (char*)s->data[1];
  ASSERT_NE(nullptr, string) << "String pointer should not be null";
  // Munit: assert_string_equal(string, "...", ...)
  // GTest: ASSERT_STREQ(expected, actual)
  ASSERT_STREQ("Sneklang is blazingly slow!", string) << "char* is equal";

  // Cleanup
  free(f);
  free(string);
  stack_free(s);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}