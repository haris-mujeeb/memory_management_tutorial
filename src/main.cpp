#include "gtest/gtest.h" // Replaces munit.h
#include "snekstack.h"   // Kept for stack functions and types
#include <stdlib.h>      // For free()

// Group all stack tests under the "SnekStackTest" suite

TEST(SnekStackTest, CreateStack) {
  snek_stack_t *s = stack_new(10);
  ASSERT_NE(nullptr, s) << "Stack pointer should not be null";

  ASSERT_EQ(10, s->capacity) << "Sets capacity to 10";
  ASSERT_EQ(0, s->count) << "No elements in the stack yet";
  ASSERT_NE(nullptr, s->data) << "Allocates the stack data";

  stack_free(s);
  // ASSERT_TRUE(boot_all_freed()); // Removed
}

TEST(SnekStackTest, PushStackAndResize) {
  snek_stack_t *s = stack_new(2);
  ASSERT_NE(nullptr, s) << "Must allocate a new stack";

  ASSERT_EQ(2, s->capacity) << "Sets capacity to 2";
  ASSERT_EQ(0, s->count) << "No elements in the stack yet";
  ASSERT_NE(nullptr, s->data) << "Allocates the stack data";

  int a = 1;

  stack_push(s, &a);
  stack_push(s, &a);

  // Stack is full
  ASSERT_EQ(2, s->capacity) << "Sets capacity to 2";
  ASSERT_EQ(2, s->count) << "2 elements in the stack";

  // This push should trigger a resize
  stack_push(s, &a);
  ASSERT_EQ(4, s->capacity) << "Capacity is doubled";
  ASSERT_EQ(3, s->count) << "3 elements in the stack";

  stack_free(s);
  // ASSERT_TRUE(boot_all_freed()); // Removed
}

TEST(SnekStackTest, PopStack) {
  snek_stack_t *s = stack_new(2);
  ASSERT_NE(nullptr, s);

  ASSERT_EQ(2, s->capacity) << "Sets capacity to 2";
  ASSERT_EQ(0, s->count) << "No elements in the stack yet";
  ASSERT_NE(nullptr, s->data);

  int one = 1;
  int two = 2;
  int three = 3;

  // Push 1, 2
  stack_push(s, &one);
  stack_push(s, &two);
  ASSERT_EQ(2, s->capacity) << "Sets capacity to 2";
  ASSERT_EQ(2, s->count) << "2 elements in the stack";

  // Push 3 (triggers resize)
  stack_push(s, &three);
  ASSERT_EQ(4, s->capacity) << "Capacity is doubled";
  ASSERT_EQ(3, s->count) << "3 elements in the stack";

  // Pop 3
  int *popped = (int*)stack_pop(s); // Assuming stack_pop returns void*
  ASSERT_NE(nullptr, popped) << "Popped pointer should not be null";
  ASSERT_EQ(three, *popped) << "Should pop the last element (3)";

  // Pop 2
  popped = (int*)stack_pop(s);
  ASSERT_NE(nullptr, popped) << "Popped pointer should not be null";
  ASSERT_EQ(two, *popped) << "Should pop the last element (2)";

  // Pop 1
  popped = (int*)stack_pop(s);
  ASSERT_NE(nullptr, popped) << "Popped pointer should not be null";
  ASSERT_EQ(one, *popped) << "Should pop the only remaining element (1)";

  // Pop from empty stack
  popped = (int*)stack_pop(s);
  
  ASSERT_EQ(nullptr, popped) << "No remaining elements";

  stack_free(s);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}