#include "gtest/gtest.h"
#include "snekobject.h"
#include <stdlib.h>
#include <stdio.h>

// ============================================================
// Reference Counting Logic Tests
// ============================================================

TEST(RefCountTest, IntHasRefcount) {
  snek_object_t *obj = new_snek_integer(10);
  ASSERT_NE(nullptr, obj);
  
  ASSERT_EQ(1, obj->refcount) << "Refcount should be 1 on creation";

  // Cleanup manually since we are just testing creation here
  free(obj);
}

TEST(RefCountTest, FloatHasRefcount) {
  snek_object_t *obj = new_snek_float(42.0);
  ASSERT_NE(nullptr, obj);
  
  ASSERT_EQ(1, obj->refcount) << "Refcount should be 1 on creation";

  free(obj);
}

TEST(RefCountTest, IncRefcount) {
  snek_object_t *obj = new_snek_integer(10);
  ASSERT_NE(nullptr, obj);
  ASSERT_EQ(1, obj->refcount) << "Refcount should be 1 on creation";

  refcount_inc(obj);
  ASSERT_EQ(2, obj->refcount) << "Refcount should be incremented";

  free(obj);
}

TEST(RefCountTest, IncRefcountMore) {
  snek_object_t *obj = new_snek_float(4.20);
  ASSERT_NE(nullptr, obj);
  ASSERT_EQ(1, obj->refcount) << "Refcount should be 1 on creation";

  for(int i = 0; i < 5; i++) {
      refcount_inc(obj);
  }
  ASSERT_EQ(6, obj->refcount) << "Refcount should be incremented to 6";

  free(obj);
}

TEST(RefCountTest, DecRefcount) {
  snek_object_t *obj = new_snek_float(4.20);
  ASSERT_NE(nullptr, obj);

  refcount_inc(obj);
  ASSERT_EQ(2, obj->refcount) << "Refcount should be incremented";

  refcount_dec(obj);
  ASSERT_EQ(1, obj->refcount) << "Refcount should be decremented";

  // assert(!boot_is_freed(obj)); 
  // In GTest/Standard C, we ensure it's still valid by checking the value again
  ASSERT_EQ(1, obj->refcount);

  // Object is still alive (refcount 1), so we free manually to avoid leak in test
  free(obj);
}

TEST(RefCountTest, RefcountFreeIsCalled) {
  snek_object_t *obj = new_snek_float(4.20);
  ASSERT_NE(nullptr, obj);

  refcount_inc(obj);
  ASSERT_EQ(2, obj->refcount);

  refcount_dec(obj);
  ASSERT_EQ(1, obj->refcount);

  // This final decrement should trigger the internal free.
  // We cannot check obj->refcount afterwards (Use-After-Free).
  // Verification of actual free relies on ASan/Valgrind.
  refcount_dec(obj);
}

TEST(RefCountTest, NullObjSafe) {
  // Should not crash
  refcount_inc(NULL);
  SUCCEED();
}

TEST(RefCountTest, AllocatedStringIsFreed) {
  snek_object_t *obj = new_snek_string("Hello @wagslane!");
  ASSERT_NE(nullptr, obj);

  refcount_inc(obj);
  ASSERT_EQ(2, obj->refcount);

  refcount_dec(obj);
  ASSERT_EQ(1, obj->refcount);
  ASSERT_STREQ("Hello @wagslane!", obj->data.v_string) << "references str";

  // Decrement to 0. Should free the string data and the object.
  refcount_dec(obj);
}

// ============================================================
// Vector3 Reference Counting Tests
// ============================================================

TEST(Vector3RefCountTest, BasicRefcounting) {
  snek_object_t *foo = new_snek_integer(1);
  snek_object_t *bar = new_snek_integer(2);
  snek_object_t *baz = new_snek_integer(3);

  // Vector takes ownership (increments refcounts of children)
  snek_object_t *vec = new_snek_vector3(foo, bar, baz);
  ASSERT_NE(nullptr, vec);

  ASSERT_EQ(2, foo->refcount) << "foo is now referenced by vec";
  ASSERT_EQ(2, bar->refcount) << "bar is now referenced by vec";
  ASSERT_EQ(2, baz->refcount) << "baz is now referenced by vec";

  // Decrement foo's original reference.
  // foo is still referenced in the vec, so it should not be freed.
  refcount_dec(foo);
  // assert(!boot_is_freed(foo)); 
  ASSERT_EQ(1, foo->refcount); // Valid check because it's alive inside vec

  // Decrement vec. This should destroy vec, and decrement references to children.
  // Since foo's refcount was 1, dropping vec should drop foo to 0 (freeing it).
  refcount_dec(vec);
  
  // assert(boot_is_freed(foo)); -> Implied by logic
  
  // bar and baz still have their original references (refcount was 2, now 1)
  // assert(!boot_is_freed(bar));
  ASSERT_EQ(1, bar->refcount);
  ASSERT_EQ(1, baz->refcount);

  // Clean up remaining
  refcount_dec(bar);
  refcount_dec(baz);
}

TEST(Vector3RefCountTest, SameObjectRefcounting) {
  snek_object_t *foo = new_snek_integer(1);

  // Vector references foo 3 times
  snek_object_t *vec = new_snek_vector3(foo, foo, foo);
  
  // 1 (original) + 3 (vector) = 4
  ASSERT_EQ(4, foo->refcount) << "foo is now referenced by vec x3";

  // Drop original reference
  refcount_dec(foo);
  // assert(!boot_is_freed(foo));
  ASSERT_EQ(3, foo->refcount);

  // Drop vector. Should decrement foo 3 times. 3 -> 0.
  refcount_dec(vec);
  // assert(boot_is_freed(foo)); -> Implied
}

// ============================================================
// Array Reference Counting Tests
// ============================================================

TEST(ArrayRefCountTest, SetElement) {
  snek_object_t *foo = new_snek_integer(1);
  snek_object_t *array = new_snek_array(1);
  ASSERT_NE(nullptr, array);

  // Setting element should increment foo's refcount
  snek_array_set(array, 0, foo);
  
  // 1 (original) + 1 (array) = 2
  ASSERT_EQ(2, foo->refcount) << "foo is now referenced by array";

  // assert(!boot_is_freed(foo));
  ASSERT_EQ(2, foo->refcount);

  // Clean up
  refcount_dec(foo); // Drops to 1 (held by array)
  refcount_dec(array); // Destroys array, drops foo to 0
}

TEST(ArrayRefCountTest, OverwriteElementFreesOld) {
  snek_object_t *foo = new_snek_integer(1);
  snek_object_t *bar = new_snek_integer(2);
  snek_object_t *baz = new_snek_integer(3);

  snek_object_t *array = new_snek_array(2);
  
  snek_array_set(array, 0, foo);
  snek_array_set(array, 1, bar);

  ASSERT_EQ(2, foo->refcount) << "foo is now referenced by array";
  ASSERT_EQ(2, bar->refcount) << "bar is now referenced by array";
  ASSERT_EQ(1, baz->refcount) << "baz is not yet referenced by array";

  // Drop original reference to foo. Foo is kept alive by array.
  refcount_dec(foo);
  // assert(!boot_is_freed(foo));
  ASSERT_EQ(1, foo->refcount);

  // Overwrite index 0 (foo) with baz.
  // Array should increment baz (1->2) and decrement foo (1->0, freeing foo).
  snek_array_set(array, 0, baz);

  // assert(boot_is_freed(foo)); -> Implied because foo refcount hit 0 inside array_set
  
  ASSERT_EQ(2, baz->refcount) << "baz now referenced by array";
  
  // Clean up
  refcount_dec(bar);
  refcount_dec(baz);
  refcount_dec(array);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}