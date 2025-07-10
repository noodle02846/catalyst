# Project setup
cmake_minimum_required(VERSION 3.13)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

set(PROJECT_VERSION_TAG "alpha")

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_VERSION_FULL "${PROJECT_VERSION}-${PROJECT_VERSION_TAG}")

project(Catalyst VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define preprocessor macro definitions
add_definitions(-DVERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DVERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DVERSION_PATCH=${PROJECT_VERSION_PATCH})
add_definitions(-DVERSION_FULL="${PROJECT_VERSION}")

# Output paths for binaries/libraries
if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
endif()

# External libraries
add_subdirectory("external")

# Project source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Project executable target
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
else()
    add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL ${SOURCE_FILES})
endif()

# Target includes
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Target libraries
target_link_libraries(${PROJECT_NAME} PRIVATE chess::chess)

# Compiler options
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "-W4")
endif()

# Linker options
if (MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/subsystem:CONSOLE")
endif()

# Target properties
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "catalyst")