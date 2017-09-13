cmake_minimum_required(VERSION 2.8.1)
project(Voronoi)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Voronoi_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Build unit tests." OFF)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build")

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

# Now simply link your own targets against gtest, gmock,
# etc. as appropriate

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
add_executable(thiessen src/main.cc
                        src/voronoi.cc
                        src/point.cc
                        src/line.cc
                        src/dcel.cc
                        src/circle.cc
                        src/graphics.cc)
target_link_libraries(thiessen ${SDL2_LIBRARY})


if(BUILD_TESTS)
    enable_testing()
    add_executable(sample_test tests/sample1_unittest.cc
                               src/voronoi.cc
                               src/point.cc
                               src/line.cc
                               src/dcel.cc
                               src/circle.cc)
    target_link_libraries(sample_test pthread)
    target_link_libraries(sample_test gtest
                                      gtest_main)
    add_test(tests sample_test)
endif(BUILD_TESTS)

