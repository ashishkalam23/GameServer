# Test Builds
# --------------------------------------------------------
enable_testing()

# The directory of source files
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# GameTests
add_executable(GameTests
        GameTest.cpp
        ${SOURCE_DIR}/State/Game.cpp
)

target_link_libraries(GameTests
        gtest
        gtest_main
        373-24-cachupa
)

# ServerTests
add_executable(ServerTests
        ServerTest.cpp
        ${SOURCE_DIR}/MessageHandler.cpp
)

target_link_libraries(ServerTests
        gtest
        gtest_main
        373-24-cachupa
)

# PlayerTests
add_executable(PlayerTests
        PlayerTest.cpp
)

target_link_libraries(PlayerTests
        gtest
        gtest_main
        373-24-cachupa
)

# Register tests with ctest
add_test(NAME GameTests COMMAND GameTests)
add_test(NAME ServerTests COMMAND ServerTests)
add_test(NAME PlayerTests COMMAND PlayerTests)

# Apply coverage flags to ServerTests if enabled
if (ENABLE_COVERAGE)
  set_target_properties(ServerTests PROPERTIES
          COMPILE_FLAGS "${CXX_COVERAGE_FLAGS}"
          LINK_FLAGS "${CXX_COVERAGE_FLAGS}"
  )
endif()
