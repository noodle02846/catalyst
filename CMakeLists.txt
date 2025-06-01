# Basic project setup
cmake_minimum_required(VERSION 3.12)
project(Catalyst VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "Setting build type to Release")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Set the output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add the source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add the include directories
include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src"
)

# Add the executable target
add_executable(Catalyst WIN32 ${SOURCE_FILES})

# Set correct linker flags for Windows
if(WIN32)
    target_link_options(Catalyst PRIVATE "/subsystem:CONSOLE")
endif()

# Set the output name for the executable
set_target_properties(Catalyst PROPERTIES OUTPUT_NAME "Catalyst")