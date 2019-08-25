cmake_minimum_required(VERSION 3.13)
project(flt LANGUAGES CXX)

# Set the build type if it has not already been provided
# Build type can be set by adding "-DCMAKE_BUILD_TYPE=Release" when calling cmake
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif()
message(STATUS "Flt - Compiling in ${CMAKE_BUILD_TYPE} mode.")

if (UNIX AND NOT APPLE)
    set(LINUX True)
endif()

# Options available to developers - set to reasonable default values
option(LIBFLT_BUILD_TESTS "Build tests" ON)

# Print the options used for clarity
message(STATUS "------------------------------------------")
message(STATUS "Libflt Build Options:")
message(STATUS "  Build Tests  - ${LIBFLT_BUILD_TESTS}")
message(STATUS "------------------------------------------")
message(STATUS "")

if (LIBFLT_BUILD_TESTS)
    message(STATUS "Building Libflt tests")
    add_subdirectory(tests)
endif()

# Define the 'flt' static library
set(DEBUG_OPTIONS -g)
set(RELEASE_OPTIONS -O3 -flto)
add_library(flt INTERFACE)
target_include_directories(flt
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_options(flt INTERFACE -Wall -Wextra)
target_compile_options(flt INTERFACE "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
target_compile_options(flt INTERFACE "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
target_compile_features(flt INTERFACE cxx_std_17)
