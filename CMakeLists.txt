cmake_minimum_required(VERSION 3.10)

project(QuadTree CXX)

option(SFML_BUILD_AUDIO OFF)
option(SFML_BUILD_NETWORK OFF)
option(SFML_BUILD_EXAMPLES OFF)
option(SFML_BUILD_DOC OFF)
add_subdirectory(lib/SFML)

set(SRC_DIR "src/")
add_executable(QuadTree ${SRC_DIR}/main.cpp)
target_include_directories(QuadTree PRIVATE lib/SFML/include)
target_link_libraries(QuadTree PRIVATE sfml-graphics)
target_compile_features(QuadTree PRIVATE cxx_std_17)
