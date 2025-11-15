#include "gtest/gtest.h"
#include "exercise.h"

// Group all tests for the snek_zero_out function under the Test Suite "SnekZeroOutTest"

TEST(SnekZeroOutTest, ZeroOutInteger) {
  snek_int_t integer;
  integer.value = 42;
  snek_zero_out(&integer, INTEGER);
  
  // Munit: munit_assert_int(integer.value, ==, 0, ...);
  // GTest: ASSERT_EQ(expected, actual)
  ASSERT_EQ(0, integer.value);
}

TEST(SnekZeroOutTest, ZeroOutFloat) {
  snek_float_t float_num;
  float_num.value = 3.14;
  snek_zero_out(&float_num, FLOAT);
  
  // Munit: munit_assert_float(float_num.value, ==, 0.0, ...);
  // GTest: ASSERT_FLOAT_EQ(expected, actual) for floating-point values
  ASSERT_FLOAT_EQ(0.0, float_num.value);
}

TEST(SnekZeroOutTest, ZeroOutBool) {
  snek_bool_t boolean;
  boolean.value = 1;
  snek_zero_out(&boolean, BOOL);
  
  // Munit: munit_assert_int(boolean.value, ==, 0, ...);
  // GTest: ASSERT_EQ(expected, actual)
  ASSERT_EQ(0, boolean.value);
}

TEST(SnekZeroOutTest, ZeroOutNonZeroValues) {
  snek_int_t integer;
  snek_float_t float_num;
  snek_bool_t boolean;

  integer.value = -100;
  float_num.value = -99.99;
  boolean.value = 255;

  // Zero out all three types
  snek_zero_out(&integer, INTEGER);
  snek_zero_out(&float_num, FLOAT);
  snek_zero_out(&boolean, BOOL);

  // Check all three results in the same test
  ASSERT_EQ(0, integer.value) << "Negative integer should be zeroed out to 0";
  ASSERT_FLOAT_EQ(0.0, float_num.value) << "Negative float should be zeroed out to 0.0";
  ASSERT_EQ(0, boolean.value) << "Non-zero boolean should be zeroed out to 0";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}