#include "gtest/gtest.h"
#include "exercise.h"
#include <stdlib.h>
#include <string.h>

// Assuming token_t structure and create_token_pointer_array function are defined in exercise.h
/*
typedef struct {
    char *literal;
    int line;
    int column;
} token_t;
token_t** create_token_pointer_array(const token_t *tokens, size_t count);
*/

// Group all tests under the Test Suite "TokenPointerArrayTest"

TEST(TokenPointerArrayTest, CreateTokenPointerArraySingle) {
  token_t token = {"hello", 1, 1};
  token_t** result = create_token_pointer_array(&token, 1);

  // Checks the result array pointer
  ASSERT_NE(nullptr, result) << "Result array should not be null";
  // Checks the first element pointer
  ASSERT_NE(nullptr, result[0]) << "First token pointer should not be null";
  
  // Checks the content of the copied token
  ASSERT_STREQ("hello", result[0]->literal) << "Literal should match";
  ASSERT_EQ(1, result[0]->line) << "Line number should match";
  ASSERT_EQ(1, result[0]->column) << "Column number should match";

  // Checks that the token was deep-copied (new memory location)
  ASSERT_NE(result[0], &token) << "Token pointer should not point to original token";

  // Cleanup
  free(result[0]);
  free(result);
}

TEST(TokenPointerArrayTest, CreateTokenPointerArrayMultiple) {
  token_t tokens[3] = {
    {"foo", 1, 1},
    {"bar", 2, 5},
    {"baz", 3, 10}
  };
  token_t** result = create_token_pointer_array(tokens, 3);

  ASSERT_NE(nullptr, result) << "Result array should not be null";

  for (int i = 0; i < 3; i++) {
    ASSERT_NE(nullptr, result[i]) << "Token pointer should not be null at index " << i;
    
    // Check content
    ASSERT_STREQ(tokens[i].literal, result[i]->literal) << "Literal mismatch at index " << i;
    ASSERT_EQ(tokens[i].line, result[i]->line) << "Line mismatch at index " << i;
    ASSERT_EQ(tokens[i].column, result[i]->column) << "Column mismatch at index " << i;
    
    // Check deep copy
    ASSERT_NE(result[i], &tokens[i]) << "Token pointer should not point to original token at index " << i;
  }

  // Cleanup
  for (int i = 0; i < 3; i++) {
    free(result[i]);
  }
  free(result);
}

TEST(TokenPointerArrayTest, MemoryAllocation) {
  token_t tokens[2] = {
    {"test1", 1, 1},
    {"test2", 2, 2}
  };
  token_t** result = create_token_pointer_array(tokens, 2);

  ASSERT_NE(nullptr, result) << "Result array should not be null";
  ASSERT_NE(nullptr, result[0]) << "First token pointer should not be null";
  ASSERT_NE(nullptr, result[1]) << "Second token pointer should not be null";
  
  // Check memory allocation properties
  ASSERT_NE(result[0], result[1]) << "Token pointers should be different (different copies)";
  ASSERT_NE(result[0], &tokens[0]) << "First token pointer should not point to original token";
  ASSERT_NE(result[1], &tokens[1]) << "Second token pointer should not point to original token";

  // Cleanup
  free(result[0]);
  free(result[1]);
  free(result);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}