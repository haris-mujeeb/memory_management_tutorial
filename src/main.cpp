#include "gtest/gtest.h"
#include "snekobject.h"
#include <stdlib.h>

// ------------------------------------------------------------
// Test Suite 1: Integer Definition & Manual Construction
// ------------------------------------------------------------

TEST(SnekObjectDefTest, IntegerConstant) {
  // Munit: assert_int(INTEGER, ==, 0, "INTEGER is defined as 0");
  ASSERT_EQ(0, INTEGER) << "INTEGER is defined as 0";
}

TEST(SnekObjectDefTest, ManualIntegerObj) {
  snek_object_t *obj = (snek_object_t*)malloc(sizeof(snek_object_t));
  ASSERT_NE(nullptr, obj) << "Malloc should succeed";

  obj->kind = INTEGER;
  obj->data.v_int = 0;

  ASSERT_EQ(INTEGER, obj->kind) << "must be INTEGER type";
  ASSERT_EQ(0, obj->data.v_int) << "must equal zero";

  free(obj);
}

// ------------------------------------------------------------
// Test Suite 2: Integer Object Helper Function (new_snek_integer)
// ------------------------------------------------------------

TEST(SnekIntegerTest, Positive) {
  snek_object_t *int_object = new_snek_integer(42);
  ASSERT_NE(nullptr, int_object);

  ASSERT_EQ(42, int_object->data.v_int) << "must allow positive numbers";

  free(int_object);
}

TEST(SnekIntegerTest, Zero) {
  snek_object_t *int_object = new_snek_integer(0);
  ASSERT_NE(nullptr, int_object);

  ASSERT_EQ(INTEGER, int_object->kind) << "must be INTEGER type";
  ASSERT_EQ(0, int_object->data.v_int) << "must equal zero";

  free(int_object);
}

TEST(SnekIntegerTest, Negative) {
  snek_object_t *int_object = new_snek_integer(-5);
  ASSERT_NE(nullptr, int_object);

  ASSERT_EQ(INTEGER, int_object->kind) << "must be INTEGER type";
  ASSERT_EQ(-5, int_object->data.v_int) << "must allow negative numbers";

  free(int_object);
}

// ------------------------------------------------------------
// Test Suite 3: Float Object Helper Function (new_snek_float)
// ------------------------------------------------------------

TEST(SnekFloatTest, Positive) {
  snek_object_t *obj = new_snek_float(42.0);
  ASSERT_NE(nullptr, obj);

  // Use ASSERT_FLOAT_EQ for float comparisons
  ASSERT_FLOAT_EQ(42.0, obj->data.v_float) << "Must accept positive values";

  free(obj);
}

TEST(SnekFloatTest, Zero) {
  snek_object_t *obj = new_snek_float(0.0);
  ASSERT_NE(nullptr, obj);

  // Note: obj->kind is likely an int/enum, so we use ASSERT_EQ
  ASSERT_EQ(FLOAT, obj->kind) << "Must set type to FLOAT";
  ASSERT_FLOAT_EQ(0.0, obj->data.v_float) << "Must accept 0.0";

  free(obj);
}

TEST(SnekFloatTest, Negative) {
  snek_object_t *obj = new_snek_float(-5.0);
  ASSERT_NE(nullptr, obj);

  ASSERT_EQ(FLOAT, obj->kind) << "Must set type to FLOAT";
  ASSERT_FLOAT_EQ(-5.0, obj->data.v_float) << "Must accept negative numbers";

  free(obj);
}

// ------------------------------------------------------------
// Test Suite 4: Vector3 Object Helper Function (new_snek_vector3)
// ------------------------------------------------------------

TEST(SnekVector3Test, ReturnsNullOnNullInput) {
  // Munit: snek_object_t *vec = new_snek_vector3(NULL, NULL, NULL);
  snek_object_t *vec = new_snek_vector3(nullptr, nullptr, nullptr);

  // Munit: assert_null(vec, "Should return null when input is null");
  // GTest: ASSERT_EQ(nullptr, actual)
  ASSERT_EQ(nullptr, vec) << "Should return null when input is null";
}

TEST(SnekVector3Test, MultipleObjects) {
  // Create three separate integer objects
  snek_object_t *x = new_snek_integer(1);
  snek_object_t *y = new_snek_integer(2);
  snek_object_t *z = new_snek_integer(3);

  snek_object_t *vec = new_snek_vector3(x, y, z);
  ASSERT_NE(nullptr, vec) << "should allocate a new vector object";

  // Vectors should not copy objects, they get the reference to the objects.
  // Munit: assert_ptr(x, ==, vec->data.v_vector3.x, "should reference x");
  // GTest: ASSERT_EQ(expected, actual)
  ASSERT_EQ(x, vec->data.v_vector3.x) << "should reference x";
  ASSERT_EQ(y, vec->data.v_vector3.y) << "should reference y";
  ASSERT_EQ(z, vec->data.v_vector3.z) << "should reference z";

  // Assert we have integer values correct
  ASSERT_EQ(1, vec->data.v_vector3.x->data.v_int) << "should have correct x value";
  ASSERT_EQ(2, vec->data.v_vector3.y->data.v_int) << "should have correct y value";
  ASSERT_EQ(3, vec->data.v_vector3.z->data.v_int) << "should have correct z value";

  // Free all of our objects (required because the vector only stores references).
  free(x);
  free(y);
  free(z);
  free(vec);
}

TEST(SnekVector3Test, SameObjectReference) {
  snek_object_t *i = new_snek_integer(1);
  snek_object_t *vec = new_snek_vector3(i, i, i);
  ASSERT_NE(nullptr, vec) << "should allocate a new vector object";

  // Assert all vector components reference the exact same object 'i'
  ASSERT_EQ(i, vec->data.v_vector3.x) << "x should reference i";
  ASSERT_EQ(i, vec->data.v_vector3.y) << "y should reference i";
  ASSERT_EQ(i, vec->data.v_vector3.z) << "z should reference i";

  // Assert initial value
  ASSERT_EQ(1, vec->data.v_vector3.x->data.v_int) << "initial x value should be 1";

  // Update the original object 'i'
  i->data.v_int = 2;

  // Assert the vector components reflect the update instantly (due to shared reference)
  
  ASSERT_EQ(2, vec->data.v_vector3.x->data.v_int) << "x value should update to 2";
  ASSERT_EQ(2, vec->data.v_vector3.y->data.v_int) << "y value should update to 2";
  ASSERT_EQ(2, vec->data.v_vector3.z->data.v_int) << "z value should update to 2";

  // Free the objects
  free(i);
  free(vec);
}

// ------------------------------------------------------------
// Test Suite 5: Array Object Helper Function (new_snek_array)
// ------------------------------------------------------------

TEST(SnekArrayTest, CreateEmptyArray) {
  // Munit: snek_object_t *obj = new_snek_array(2);
  snek_object_t *obj = new_snek_array(2);
  ASSERT_NE(nullptr, obj) << "Array object should be allocated";

  // Assuming ARRAY is an enum/constant defined in snekobject.h
  ASSERT_EQ(ARRAY, obj->kind) << "Must set type to ARRAY";
  ASSERT_EQ(2, obj->data.v_array.size) << "Must set size to 2";
  ASSERT_NE(nullptr, obj->data.v_array.elements) << "Elements array should be allocated";

  // Cleanup: must free the elements array and the wrapper object
  free(obj->data.v_array.elements);
  free(obj);
}

TEST(SnekArrayTest, UsedCallocInitialization) {
  snek_object_t *obj = new_snek_array(2);
  ASSERT_NE(nullptr, obj) << "Array object should be allocated";

  // Munit: assert_ptr_null(obj->data.v_array.elements[0], "Should use calloc");
  // GTest: ASSERT_EQ(nullptr, actual)
  // Calloc initializes memory to zero, which means pointers will be NULL (or nullptr).
  ASSERT_EQ(nullptr, obj->data.v_array.elements[0]) << "First element must be initialized to NULL (calloc)";
  ASSERT_EQ(nullptr, obj->data.v_array.elements[1]) << "Second element must be initialized to NULL (calloc)";

  // Cleanup
  free(obj->data.v_array.elements);
  free(obj);
}

// ------------------------------------------------------------
// Test Suite 6: Array Set/Get Operations (object-array)
// ------------------------------------------------------------

TEST(SnekArrayOpsTest, SetAndGetHeterogeneous) {
  snek_object_t *obj = new_snek_array(2);
  ASSERT_NE(nullptr, obj);

  snek_object_t *first = new_snek_string("First");
  snek_object_t *second = new_snek_integer(3);
  ASSERT_NE(nullptr, first);
  ASSERT_NE(nullptr, second);

  // Munit: assert(snek_array_set(obj, 0, first));
  ASSERT_TRUE(snek_array_set(obj, 0, first));
  ASSERT_TRUE(snek_array_set(obj, 1, second));

  // Retrieve and verify first object (String)
  snek_object_t *retrieved_first = snek_array_get(obj, 0);
  ASSERT_NE(nullptr, retrieved_first) << "Should find the first object";
  ASSERT_EQ(STRING, retrieved_first->kind) << "Should be a string";
  // Check pointer equality (reference stored, not copy)
  ASSERT_EQ(first, retrieved_first) << "Should be the same object pointer";

  // Retrieve and verify second object (Integer)
  snek_object_t *retrieved_second = snek_array_get(obj, 1);
  ASSERT_NE(nullptr, retrieved_second) << "Should find the second object";
  ASSERT_EQ(INTEGER, retrieved_second->kind) << "Should be a integer";
  ASSERT_EQ(second, retrieved_second) << "Should be the same object pointer";

  // Cleanup: Requires freeing the deep-copied string data first
  free(first->data.v_string);
  free(first);
  free(second);
  free(obj->data.v_array.elements);
  free(obj);
}

TEST(SnekArrayOpsTest, SetOutsideBounds) {
  snek_object_t *obj = new_snek_array(2);
  snek_object_t *outside = new_snek_string("First");
  ASSERT_NE(nullptr, obj);
  ASSERT_NE(nullptr, outside);

  // Inside of bound (index 1 is valid for size 2 array)
  ASSERT_TRUE(snek_array_set(obj, 1, outside));

  // Outside of bound (index 100 is invalid)
  // Munit: assert_false(snek_array_set(obj, 100, outside));
  ASSERT_FALSE(snek_array_set(obj, 100, outside));

  // Free memory
  free(outside->data.v_string);
  free(outside);
  free(obj->data.v_array.elements);
  free(obj);
}

TEST(SnekArrayOpsTest, GetOutsideBounds) {
  snek_object_t *obj = new_snek_array(1);
  snek_object_t *first = new_snek_string("First");
  ASSERT_NE(nullptr, obj);
  ASSERT_TRUE(snek_array_set(obj, 0, first));

  // Outside of bound (index 1 is invalid for size 1 array)
  // Munit: assert_null(snek_array_get(obj, 1), "Should not access outside the array");
  ASSERT_EQ(nullptr, snek_array_get(obj, 1)) << "Should not access outside the array";

  // Free memory
  free(first->data.v_string);
  free(first);
  free(obj->data.v_array.elements);
  free(obj);
}

// ------------------------------------------------------------
// Test Suite 7: Object Length (snek_length)
// ------------------------------------------------------------

TEST(SnekLengthTest, Integer) {
  snek_object_t *obj = new_snek_integer(42);
  ASSERT_EQ(1, snek_length(obj)) << "Integers are length 1";
  free(obj);
}

TEST(SnekLengthTest, Float) {
  snek_object_t *obj = new_snek_float(3.14);
  ASSERT_EQ(1, snek_length(obj)) << "Floats are length 1";
  free(obj);
}

TEST(SnekLengthTest, String) {
  snek_object_t *shorter = new_snek_string("hello");
  ASSERT_EQ(5, snek_length(shorter)) << "Should return based on input length";

  snek_object_t *longer = new_snek_string("hello, world");
  ASSERT_EQ(strlen("hello, world"), snek_length(longer)) << "Should use strlen";

  // Cleanup
  free(shorter->data.v_string);
  free(shorter);
  free(longer->data.v_string);
  free(longer);
}

TEST(SnekLengthTest, Vector3) {
  snek_object_t *i = new_snek_integer(1);
  snek_object_t *vec = new_snek_vector3(i, i, i);
  ASSERT_EQ(3, snek_length(vec)) << "Vec3 always has length 3";

  // Cleanup: Vector components are only referenced, not copied.
  free(i);
  free(vec);
}

TEST(SnekLengthTest, Array) {
  snek_object_t *i = new_snek_integer(1);
  snek_object_t *arr = new_snek_array(4);
  ASSERT_TRUE(snek_array_set(arr, 0, i));
  ASSERT_TRUE(snek_array_set(arr, 1, i));
  ASSERT_TRUE(snek_array_set(arr, 2, i));

  // Length of array should be its fixed size (4)
  ASSERT_EQ(4, snek_length(arr)) << "Length of array should be its size";

  // Cleanup
  free(i);
  free(arr->data.v_array.elements);
  free(arr);
}

// ------------------------------------------------------------
// Test Suite 8: Object Addition (snek_add)
// ------------------------------------------------------------

TEST(SnekAddTest, IntegerAdd) {
  snek_object_t *one = new_snek_integer(1);
  snek_object_t *three = new_snek_integer(3);
  snek_object_t *four = snek_add(one, three);

  ASSERT_NE(nullptr, four) << "must return an object";
  ASSERT_EQ(INTEGER, four->kind) << "1 + 3 = 4";
  ASSERT_EQ(4, four->data.v_int) << "1 + 3 = 4";

  free(one);
  free(three);
  free(four);
}

TEST(SnekAddTest, FloatAdd) {
  snek_object_t *one = new_snek_float(1.5);
  snek_object_t *three = new_snek_float(3.5);
  snek_object_t *five = snek_add(one, three);

  ASSERT_NE(nullptr, five) << "must return an object";
  ASSERT_EQ(FLOAT, five->kind) << "1.5 + 3.5 = 5.0";
  ASSERT_FLOAT_EQ(5.0, five->data.v_float) << "1.5 + 3.5 = 5.0";

  free(one);
  free(three);
  free(five);
}

TEST(SnekAddTest, StringAdd) {
  snek_object_t *hello = new_snek_string("hello");
  snek_object_t *world = new_snek_string(", world");
  snek_object_t *greeting = snek_add(hello, world);

  ASSERT_NE(nullptr, greeting) << "must return an object";
  ASSERT_EQ(STRING, greeting->kind) << "Must be a string!";
  ASSERT_STREQ("hello, world", greeting->data.v_string) << "Should concatenate strings";

  // Cleanup: Original objects and the result object's internal string data
  free(hello->data.v_string);
  free(hello);
  free(world->data.v_string);
  free(world);
  free(greeting->data.v_string);
  free(greeting);
}

TEST(SnekAddTest, StringAddSelf) {
  snek_object_t *repeated = new_snek_string("(repeated)");
  snek_object_t *result = snek_add(repeated, repeated);

  ASSERT_NE(nullptr, result) << "must return an object";
  ASSERT_EQ(STRING, result->kind) << "Must be a string!";
  ASSERT_STREQ("(repeated)(repeated)", result->data.v_string) << "Should concatenate strings";

  // Cleanup
  free(repeated->data.v_string);
  free(repeated);
  free(result->data.v_string);
  free(result);
}

TEST(SnekAddTest, Vector3Add) {
  snek_object_t *one = new_snek_float(1.0);
  snek_object_t *two = new_snek_float(2.0);
  snek_object_t *three = new_snek_float(3.0);
  snek_object_t *four = new_snek_float(4.0);
  snek_object_t *five = new_snek_float(5.0);
  snek_object_t *six = new_snek_float(6.0);

  snek_object_t *v1 = new_snek_vector3(one, two, three);
  snek_object_t *v2 = new_snek_vector3(four, five, six);
  snek_object_t *result = snek_add(v1, v2);

  ASSERT_NE(nullptr, result) << "must return an object";
  ASSERT_EQ(VECTOR3, result->kind) << "Must be a vector3";

  // The result vector components must be new objects (1.0+4.0, 2.0+5.0, etc.)
  ASSERT_FLOAT_EQ(5.0, result->data.v_vector3.x->data.v_float) << "x component should be 5.0";
  ASSERT_FLOAT_EQ(7.0, result->data.v_vector3.y->data.v_float) << "y component should be 7.0";
  ASSERT_FLOAT_EQ(9.0, result->data.v_vector3.z->data.v_float) << "z component should be 9.0";

  // Cleanup: Free all original components, original vectors, and result components/vector.
  // Original components (one through six) are referenced by v1 and v2, but the snek_add result is new.
  // The structure of the cleanup is simplified here as per the Munit code structure:
  free(v1->data.v_vector3.x); free(v1->data.v_vector3.y); free(v1->data.v_vector3.z); free(v1);
  free(v2->data.v_vector3.x); free(v2->data.v_vector3.y); free(v2->data.v_vector3.z); free(v2);

  free(result->data.v_vector3.x); free(result->data.v_vector3.y); free(result->data.v_vector3.z); free(result);
}

TEST(SnekAddTest, ArrayAdd) {
  snek_object_t *one = new_snek_integer(1);
  snek_object_t *ones = new_snek_array(2);
  ASSERT_TRUE(snek_array_set(ones, 0, one));
  ASSERT_TRUE(snek_array_set(ones, 1, one));

  snek_object_t *hi = new_snek_string("hi");
  snek_object_t *hellos = new_snek_array(3);
  ASSERT_TRUE(snek_array_set(hellos, 0, hi));
  ASSERT_TRUE(snek_array_set(hellos, 1, hi));
  ASSERT_TRUE(snek_array_set(hellos, 2, hi));

  snek_object_t *result = snek_add(ones, hellos);

  ASSERT_NE(nullptr, result) << "must return an object";
  ASSERT_EQ(ARRAY, result->kind) << "Must be an array";
  ASSERT_EQ(5, result->data.v_array.size) << "Result size must be 2 + 3 = 5";

  // Check concatenation (1, 1, hi, hi, hi)
  snek_object_t *first = snek_array_get(result, 0);
  ASSERT_NE(nullptr, first) << "should find the first item";
  ASSERT_EQ(INTEGER, first->kind);
  ASSERT_EQ(1, first->data.v_int) << "First item should be an int with 1";

  snek_object_t *third = snek_array_get(result, 2);
  ASSERT_NE(nullptr, third) << "should find the third item";
  ASSERT_EQ(STRING, third->kind);
  ASSERT_STREQ("hi", third->data.v_string) << "third == hi";
  
  // Cleanup
  free(one);
  free(ones->data.v_array.elements);
  free(ones);

  free(hi->data.v_string);
  free(hi);
  free(hellos->data.v_array.elements);
  free(hellos);
  
  // Clean up result array and its copied elements
  // Assuming snek_add copies the pointers/objects into the result array:
  for (size_t i = 0; i < result->data.v_array.size; ++i) {
      if (i < 2) free(result->data.v_array.elements[i]); // Free copied integers
      else {
          free(result->data.v_array.elements[i]->data.v_string); // Free copied string data
          free(result->data.v_array.elements[i]); // Free copied string object
      }
  }

  free(result->data.v_array.elements);
  free(result);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}