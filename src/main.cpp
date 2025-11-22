#include "gtest/gtest.h"
#include "snekobject.h"
#include "vm.h"
#include "sneknew.h"

#include <stdlib.h>
#include <stdio.h>

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


TEST_F(VMTest, TestVMNew) {
  ASSERT_EQ(vm->frames->capacity, 8) << "frames should have capacity 8";
  ASSERT_EQ(vm->objects->capacity, 8) << "objects should have capacity 8";
}


TEST_F(VMTest, TestVMFree) {
  // For a simple existence check:
  ASSERT_NE(vm, nullptr);
}


TEST(VMFrameTest, TestVMNewFrame) {
  vm_t *vm = vm_new();
  ASSERT_NE(vm, nullptr);
  
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