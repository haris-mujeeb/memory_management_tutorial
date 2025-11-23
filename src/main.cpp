#include "gtest/gtest.h"
#include "snekobject.h"
#include "vm.h"
#include "sneknew.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ============================================================
// Mark and Sweep Tests
// ============================================================

class VMTest : public ::testing::Test {
  protected:
    vm_t *vm = nullptr;

    void SetUp() override {
      vm = vm_new();
    }

    void TearDown() override {
      vm_free(vm);
      vm = nullptr;
    }
};


TEST_F(VMTest, TestMarkFieldExists) {
  // Munit: test_field_exists
  
  // Create objects using the VM's memory manager.
  snek_object_t *lane_courses = new_snek_integer(vm, 20);
  snek_object_t *teej_courses = new_snek_integer(vm, 1);

  // The assertion here is implicit: if the code below compiles, the field exists.
  // We access the field to ensure it compiles without error.
  // This verifies the 'is_marked' field is present in snek_object_t.
  bool lane_marked = lane_courses->is_marked;
  bool teej_marked = teej_courses->is_marked;
  
  // Use the variables to prevent compiler warnings about unused variables
  (void)lane_marked;
  (void)teej_marked;

  // Cleanup is handled by TearDown().
  SUCCEED();
}

TEST_F(VMTest, TestMarkedIsFalseOnCreation) {
  // Munit: test_marked_is_false
  
  snek_object_t *lane_courses = new_snek_integer(vm, 20);
  snek_object_t *teej_courses = new_snek_integer(vm, 1);

  // Munit: assert_false(lane_courses->is_marked);
  // ASSERT_FALSE checks if the boolean value is false.
  ASSERT_FALSE(lane_courses->is_marked) << "Newly created object must not be marked (is_marked=false)";
  ASSERT_FALSE(teej_courses->is_marked) << "Newly created object must not be marked (is_marked=false)";
  
  // Cleanup is handled by TearDown().
  SUCCEED();
}

TEST_F(VMTest, TestVMNew) {
  ASSERT_EQ(vm->frames->capacity, 8) << "frames should have capacity 8";
  ASSERT_EQ(vm->objects->capacity, 8) << "objects should have capacity 8";
}

TEST_F(VMTest, TestVMFreesReferencedObjects) {
  new_snek_integer(vm, 5);
  new_snek_string(vm, (char*)"hello");

  SUCCEED();
}

TEST_F(VMTest, TestVMFreeCheck) {
  // For a simple existence check that vm_new() worked:
  ASSERT_NE(vm, nullptr);
  // vm_free() will be called automatically by TearDown(), implicitly testing cleanup.
}

TEST_F(VMTest, VMFreesFrames) {
  // The frame is created, and the automatic TearDown() calls vm_free(), 
  // which implicitly tests that the internal frame structure is correctly freed.
  vm_new_frame(vm);

  // No explicit cleanup needed here.
  SUCCEED();
}

TEST_F(VMTest, TestTraceVector) {
  frame_t *frame = vm_new_frame(vm);

  snek_object_t *x = new_snek_integer(vm, 5);
  snek_object_t *y = new_snek_integer(vm, 5);
  snek_object_t *z = new_snek_integer(vm, 5);
  snek_object_t *vector = new_snek_vector3(vm, x, y, z);

  // 1. Initial State: Nothing is marked
  ASSERT_FALSE(x->is_marked);
  ASSERT_FALSE(y->is_marked);
  ASSERT_FALSE(z->is_marked);
  ASSERT_FALSE(vector->is_marked);

  // 2. Mark Roots: Reference the vector in the frame and call mark()
  // This should mark the vector (the root), but NOT its children yet.
  frame_reference_object(frame, vector);
  mark(vm);
  
  ASSERT_TRUE(vector->is_marked) << "Vector (root) should be marked";
  ASSERT_FALSE(x->is_marked) << "Child x should not be marked yet";
  ASSERT_FALSE(y->is_marked) << "Child y should not be marked yet";
  ASSERT_FALSE(z->is_marked) << "Child z should not be marked yet";

  // 3. Trace: Traverse the graph
  // This should find x, y, z via the vector and mark them.
  trace(vm);
  
  ASSERT_TRUE(vector->is_marked);
  ASSERT_TRUE(x->is_marked) << "Child x should be marked after trace";
  ASSERT_TRUE(y->is_marked) << "Child y should be marked after trace";
  ASSERT_TRUE(z->is_marked) << "Child z should be marked after trace";
}

TEST_F(VMTest, TestTraceArray) {
  frame_t *frame = vm_new_frame(vm);

  snek_object_t *devs = new_snek_array(vm, 2);

  
  snek_object_t *lane = new_snek_string(vm, (char*)"Lane");
  snek_object_t *teej = new_snek_string(vm, (char*)"Teej");
  
  // Setup Array
  snek_array_set(devs, 0, lane);
  snek_array_set(devs, 1, teej);

  // 1. Initial State
  ASSERT_FALSE(devs->is_marked);
  ASSERT_FALSE(lane->is_marked);
  ASSERT_FALSE(teej->is_marked);

  // 2. Mark Roots
  frame_reference_object(frame, devs);
  mark(vm);
  
  ASSERT_TRUE(devs->is_marked) << "Array (root) should be marked";
  ASSERT_FALSE(lane->is_marked) << "String content should not be marked yet";
  ASSERT_FALSE(teej->is_marked) << "String content should not be marked yet";

  // 3. Trace
  trace(vm);
  
  ASSERT_TRUE(devs->is_marked);
  ASSERT_TRUE(lane->is_marked) << "String content should be marked after trace";
  ASSERT_TRUE(teej->is_marked) << "String content should be marked after trace";
}

TEST_F(VMTest, TestTraceNested) {
  frame_t *frame = vm_new_frame(vm);

  // --- Setup complex nested structure ---
  // bootdevs = ["Lane", "Hunter"]
  snek_object_t *bootdevs = new_snek_array(vm, 2);
  snek_object_t *lane = new_snek_string(vm, (char*)"Lane");
  snek_object_t *hunter = new_snek_string(vm, (char*)"Hunter");
  snek_array_set(bootdevs, 0, lane);
  snek_array_set(bootdevs, 1, hunter);

  // terminaldevs = ["Prime", "Teej", "Dax", "Adam"]
  snek_object_t *terminaldevs = new_snek_array(vm, 4);
  snek_object_t *prime = new_snek_string(vm, (char*)"Prime");
  snek_object_t *teej = new_snek_string(vm, (char*)"Teej");
  snek_object_t *dax = new_snek_string(vm, (char*)"Dax");
  snek_object_t *adam = new_snek_string(vm, (char*)"Adam");
  snek_array_set(terminaldevs, 0, prime);
  snek_array_set(terminaldevs, 1, teej);
  snek_array_set(terminaldevs, 2, dax);
  snek_array_set(terminaldevs, 3, adam);

  // alldevs = [bootdevs, terminaldevs]
  snek_object_t *alldevs = new_snek_array(vm, 2);
  snek_array_set(alldevs, 0, bootdevs);
  snek_array_set(alldevs, 1, terminaldevs);

  // --- Execute GC Mark/Trace ---
  frame_reference_object(frame, alldevs);
  mark(vm);
  trace(vm);

  // --- Assert Deep Marking ---
  ASSERT_TRUE(alldevs->is_marked) << "Root array marked";
  
  // Level 1 children
  ASSERT_TRUE(bootdevs->is_marked) << "Nested array 1 marked";
  ASSERT_TRUE(terminaldevs->is_marked) << "Nested array 2 marked";
  
  // Level 2 children (bootdevs contents)
  ASSERT_TRUE(lane->is_marked);
  ASSERT_TRUE(hunter->is_marked);
  
  // Level 2 children (terminaldevs contents)
  ASSERT_TRUE(prime->is_marked);
  ASSERT_TRUE(teej->is_marked);
  ASSERT_TRUE(dax->is_marked);
  ASSERT_TRUE(adam->is_marked);
}

TEST_F(VMTest, TestSimpleGC) {
  // Munit: test_simple
  frame_t *f1 = vm_new_frame(vm);
  
  // Create a string and reference it in the stack frame
  snek_object_t *s = new_snek_string(vm, (char*)"I wish I knew how to read.");
  frame_reference_object(f1, s);
  
  // Sanity check: 1 object exists
  ASSERT_EQ(1, vm->objects->count);
  
  // Trigger GC
  vm_collect_garbage(vm);
  
  // Munit: assert(!boot_is_freed(s));
  // GTest: The object is referenced by f1, so it should NOT be collected.
  // The count remaining 1 proves the object is still alive and tracked.
  ASSERT_EQ(1, vm->objects->count) << "Object should survive GC because frame is active";

  // Remove the frame (popping the root reference)
  // 
  frame_free(vm_frame_pop(vm));
  
  // Trigger GC again
  vm_collect_garbage(vm);
  
  // Munit: assert_true(boot_is_freed(s));
  // GTest: The object is now unreachable. The GC should sweep it.
  // Count decreasing to 0 proves the object was freed.
  ASSERT_EQ(0, vm->objects->count) << "Object should be collected after frame pop";
}

TEST_F(VMTest, TestFullGCScenarios) {
  // Munit: test_full
  
  // Setup 3 frames
  frame_t *f1 = vm_new_frame(vm);
  frame_t *f2 = vm_new_frame(vm);
  frame_t *f3 = vm_new_frame(vm);

  // Create Strings
  snek_object_t *s1 = new_snek_string(vm, (char*)"This string is going into frame 1");
  frame_reference_object(f1, s1);

  snek_object_t *s2 = new_snek_string(vm, (char*)"This string is going into frame 2");
  frame_reference_object(f2, s2);

  snek_object_t *s3 = new_snek_string(vm, (char*)"This string is going into frame 3");
  frame_reference_object(f3, s3);

  // Create Vector and its contents
  snek_object_t *i1 = new_snek_integer(vm, 69);
  snek_object_t *i2 = new_snek_integer(vm, 420);
  snek_object_t *i3 = new_snek_integer(vm, 1337);
  snek_object_t *v = new_snek_vector3(vm, i1, i2, i3);
  
  // Vector is referenced by BOTH frame 2 and frame 3
  frame_reference_object(f2, v);
  frame_reference_object(f3, v);

  // Verify initial state
  // Objects: s1, s2, s3, i1, i2, i3, v = 7 objects
  ASSERT_EQ(7, vm->objects->count) << "Correct number of objects in the VM before GC";

  // --- Step 1: Free Top Frame (f3) ---
  // f3 held references to: s3, v
  frame_free(vm_frame_pop(vm));
  vm_collect_garbage(vm);

  // s3: Was only in f3. Should be FREED.
  // v:  Was in f3, BUT is still in f2. Should be ALIVE.
  // s1, s2: Still in f1, f2. ALIVE.
  // i1..i3: Referenced by v. ALIVE.
  
  // Expected count: 7 - 1 (s3) = 6
  ASSERT_EQ(6, vm->objects->count) << "Only s3 should be collected (v is still held by f2)";

  // --- Step 2: Free Remaining Frames ---
  frame_free(vm_frame_pop(vm)); // Pops f2 (releases s2, v)
  frame_free(vm_frame_pop(vm)); // Pops f1 (releases s1)
  
  vm_collect_garbage(vm);

  // Now s1, s2 are unreachable.
  // v is unreachable (f2 gone).
  // i1..i3 are unreachable (v gone).
  
  // Munit checked boot_is_freed for all.
  // GTest verifies the VM object list is completely empty.
  ASSERT_EQ(0, vm->objects->count) << "No live objects remaining";
}

TEST(VMFrameTest, TestVMNewFrameInitialization) {
  vm_t *vm = vm_new();
  ASSERT_NE(vm, nullptr);
  
  // Action: Create and check the frame's internal stack state
  frame_t *frame = vm_new_frame(vm);
  ASSERT_NE(frame->references, nullptr) << "frame->references must be allocated";
  
  ASSERT_EQ(frame->references->count, 0) << "references stack should start empty";
  ASSERT_GT(frame->references->capacity, 0); // references stack must have capacity > 0
  ASSERT_NE(frame->references->data, nullptr) << "references stack backing array must be allocated";
  
  vm_free(vm);
}

TEST(VMFrameTest, VMFreesFrames) {
  vm_t *vm  = vm_new();
  ASSERT_NE(nullptr, vm);
  
  vm_new_frame(vm);
  
  // Implicitly checks that vm_free correctly cleans up all allocated frames
  vm_free(vm);
  
  // Success relies on ASan/Valgrind when running the test executable.
  SUCCEED();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}