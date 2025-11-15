#include "gtest/gtest.h" // Replaces munit.h
#include "bootlib.h"    // Kept for boot_all_freed()
#include "exercise.h"    // Kept for the swap() function
#include <stdint.h>      // Kept for uint64_t

// Struct definition from the original test file
typedef struct CoffeeShop {
  uint64_t quality;
  uint64_t taste;
  uint64_t branding;
} coffee_shop_t;

// Group all tests for the swap function under the Test Suite "SwapTest"

TEST(SwapTest, GenericInts) {
  int i1 = 1234;
  int i2 = 5678;

  swap(&i1, &i2, sizeof(int));

  // Munit: assert_int(i1, ==, 5678, ...);
  // GTest: ASSERT_EQ(expected, actual)
  ASSERT_EQ(5678, i1) << "i1 should be i2's original value";
  ASSERT_EQ(1234, i2) << "i2 should be i1's original value";
  
  // Munit: assert_true(boot_all_freed());
  ASSERT_TRUE(boot_all_freed());
}

TEST(SwapTest, GenericStrings) {
  // Note: This swaps the pointers, not the string content.
  const char *s1 = "dax";
  const char *s2 = "adam";

  swap(&s1, &s2, sizeof(char *));

  // Munit: assert_string_equal(s1, "adam", ...);
  // GTest: ASSERT_STREQ(expected, actual)
  ASSERT_STREQ("adam", s1) << "s1 should be s2's original value";
  ASSERT_STREQ("dax", s2) << "s2 should be i1's original value";
  
  ASSERT_TRUE(boot_all_freed());
}

TEST(SwapTest, GenericStructs) {
  coffee_shop_t sbucks = {2, 3, 4};
  coffee_shop_t terminalshop = {10, 10, 10};

  swap(&sbucks, &terminalshop, sizeof(coffee_shop_t));

  // Check all members of the first struct
  ASSERT_EQ(10, sbucks.quality) << "sbucks.quality should be terminalshop.quality";
  ASSERT_EQ(10, sbucks.taste) << "sbucks.taste should be terminalshop.taste";
  ASSERT_EQ(10, sbucks.branding) << "sbucks.branding should be terminalshop.branding";

  // Check all members of the second struct
  ASSERT_EQ(2, terminalshop.quality) << "terminalshop.quality should be sbucks.quality";
  ASSERT_EQ(3, terminalshop.taste) << "terminalshop.taste should be sbucks.taste";
  ASSERT_EQ(4, terminalshop.branding) << "terminalshop.branding should be sbucks.branding";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}