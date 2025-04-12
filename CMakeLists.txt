cmake_minimum_required(VERSION 3.28.1)
project(cachupa)
set(CMAKE_CXX_STANDARD 20)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# External Dependencies
# --------------------------------------------------------
# Add JsonCpp linking
find_package(jsoncpp REQUIRED)

# Fetch and include GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Tree-sitter configuration
include(cmake/CPM.cmake)
CPMAddPackage(
        NAME cpp-tree-sitter
        GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
        GIT_TAG v0.0.1
)

add_grammar_from_repo(tree-sitter-socialgaming
        https://github.com/nsumner/tree-sitter-socialgaming.git
        0.0.2
)

# Coverage Instrumentation
# --------------------------------------------------------
if (ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug")
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    set(CXX_COVERAGE_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
  else()
    message(FATAL_ERROR "Coverage generation is only supported for clang")
  endif()
  message("Enabling coverage instrumentation with:\n ${CXX_COVERAGE_FLAGS}")
endif()

# TreeSitter executable
# add_executable(TreeSitterApp
#         src/MessageHandler.cpp
#         src/TreeSitter.cpp
#         src/GameData.cpp
# )

# target_link_libraries(TreeSitterApp
#         tree-sitter-socialgaming
#         cpp-tree-sitter
#         jsoncpp_lib
# )
add_subdirectory(src)
add_subdirectory(test)