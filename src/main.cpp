#include "gtest/gtest.h"
#include "snekobject.h"
#include <stdlib.h>
#include <stdio.h>

// ============================================================
// Reference Counting Cycle Tests
// ============================================================

TEST(RefCountTest, CircularReferenceLeak) {
  snek_object_t *first = new_snek_array(1);
  snek_object_t *second = new_snek_array(1);
  // ref count: first = 1, second = 1

  snek_array_set(first, 0, second);
  // ref count: first = 1, second = 2
  snek_array_set(second, 0, first);
  // ref count: first = 2, second = 2
  
  refcount_dec(first);
  // ref count: first = 1, second = 2
  refcount_dec(second);
  // ref count: first = 1, second = 1

  ASSERT_EQ(1, first->refcount) << "First array must be leaked (refcount > 0)";
  ASSERT_EQ(1, second->refcount) << "Second array must be leaked (refcount > 0)";

  // Manual cleanup is required here to avoid an actual leak in the test runner.
  // This is technically cheating the refcounter logic being tested.
  free(first->data.v_array.elements);
  free(first);
  free(second->data.v_array.elements);
  free(second);

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}