cmake_minimum_required(VERSION 3.10)
project(MemoryManagementTutorial C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0 # Or a more recent stable release
)

FetchContent_MakeAvailable(googletest)

add_executable(memory_management_tutorial
    src/main.cpp
    src/snekobject.c
    # src/exercise.c
)

target_include_directories(memory_management_tutorial PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(memory_management_tutorial PRIVATE gtest_main)