cmake_minimum_required(VERSION 3.29)
project(preprocess)

set(CMAKE_CXX_STANDARD 26)
add_subdirectory(lua-cmake)
add_subdirectory(sol2/single)

file(GLOB_RECURSE PREPROCESS_SRC "src/*.cpp" "src/*.h")
add_executable(preprocess_binary ${PREPROCESS_SRC})
target_link_libraries(preprocess_binary PUBLIC lua lua-header sol2::sol2::single)

add_custom_target(
        preprocess_cmd
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/preprocess_binary
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(preprocess INTERFACE)
add_dependencies(preprocess preprocess_binary preprocess_cmd)
target_include_directories(preprocess INTERFACE ${CMAKE_SOURCE_DIR}/preprocess/include)

set(BUILD_EXAMPLES ON)
if(BUILD_EXAMPLES)
    add_executable(preprocess_example example/main.cpp)
    target_link_libraries(preprocess_example preprocess)
endif()