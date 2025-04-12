# Main Library
# --------------------------------------------------------
add_subdirectory(Controllers)
add_subdirectory(Network)
add_subdirectory(Parser)
add_subdirectory(State)

add_library(373-24-cachupa CachupaServer.cpp)

target_link_libraries(373-24-cachupa PRIVATE jsoncpp_lib PUBLIC Controllers Network State)

# Executable Builds
# --------------------------------------------------------
# Main executable for demo
add_executable(demo main.cpp)

target_link_libraries(demo PRIVATE 373-24-cachupa)