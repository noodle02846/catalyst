# Set compiler macros for versioning
set(CATALYST_VERSION_MAJOR 0)
set(CATALYST_VERSION_MINOR 24)
set(CATALYST_VERSION_PATCH 0)

# Pre-release label
set(CATALYST_VERSION_PRE_RELEASE "alpha")

# Full version string
set(CATALYST_VERSION "${CATALYST_VERSION_MAJOR}.${CATALYST_VERSION_MINOR}.${CATALYST_VERSION_PATCH}")
set(CATALYST_VERSION_FULL "
    ${CATALYST_VERSION_MAJOR}.
    ${CATALYST_VERSION_MINOR}.
    ${CATALYST_VERSION_PATCH}${CATALYST_VERSION_PRE_RELEASE}
")

# Basic project setup
cmake_minimum_required(VERSION 3.13)
project(Catalyst VERSION ${CATALYST_VERSION} LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define preprocessor macros for versioning
add_definitions(-DCATALYST_VERSION_MAJOR=${CATALYST_VERSION_MAJOR})
add_definitions(-DCATALYST_VERSION_MINOR=${CATALYST_VERSION_MINOR})
add_definitions(-DCATALYST_VERSION_PATCH=${CATALYST_VERSION_PATCH})

# Full preprocessor macro for version
add_definitions(-DCATALYST_VERSION="${CATALYST_VERSION}")

# Set the build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "Setting build type to Release")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Set the output directories for binaries and libraries
if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
endif()

# Add the source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add the executable target
if (MSVC)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
else()
    add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL ${SOURCE_FILES})
endif()

# Include project headers
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Include library headers
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/lib/chess-library/include")

# Set correct linker flags for Windows
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/subsystem:CONSOLE")
endif()

# Set the output name for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Catalyst")