# Copyright (c) 2020 Gustavo Salomao
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# =============================================================================
# Setup
# =============================================================================

cmake_minimum_required(VERSION 3.13)
project(cppdbc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prevent in-source build
if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. "
    "Please make a new directory (called build directory) and run CMake from "
    "there.")
endif ()

# =============================================================================
# Flags
# =============================================================================

option(BUILD_SHARED_LIBS "Build shared library" NO)
option(ENABLE_COVERAGE "Enable test coverage" NO)

if (ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
  set(ENABLE_TESTS YES)
endif ()

if (ENABLE_TESTS)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif ()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# =============================================================================
# Software Version
# =============================================================================

include(GitVersion)
message(STATUS "Project version: ${PROJECT_VERSION}")

# =============================================================================
# Libraries
# =============================================================================

include_directories(include)
include_directories(src)
add_subdirectory(src)

message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")

# =============================================================================
# Tests
# =============================================================================

option(ENABLE_TESTS "Enable tests" NO)

if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif ()

# =============================================================================
# Static Analysis
# =============================================================================

find_program(CPPCHECK cppcheck)

if (CPPCHECK)
  message(STATUS "Adding static analysis")

  add_custom_target(static)
  add_custom_command(TARGET static
    COMMAND ${CPPCHECK} -q
    --enable=all
    --suppress=missingIncludeSystem
    --suppress=unusedFunction:../src/sqlite/*.cpp
    -I../include
    -I../src
    ../src/)
else ()
  message(WARNING "No cppcheck found! Static check disabled.")
endif ()

# =============================================================================
# Installation
# =============================================================================

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
message(STATUS "Installation into ${CMAKE_INSTALL_PREFIX}")

# =============================================================================
# Documentation
# =============================================================================

option(BUILD_DOC "Build documentation" NO)

if (BUILD_DOC)
  add_subdirectory(doc)
endif ()

# =============================================================================
# Build Info
# =============================================================================

message(STATUS)
message(STATUS "-------------------------------------------------------------")
message(STATUS "                      Build Info")
message(STATUS "-------------------------------------------------------------")
message(STATUS "Project:      ${PROJECT_NAME}")
message(STATUS "Version:      ${PROJECT_VERSION}")
message(STATUS "Build:        ${CMAKE_BUILD_TYPE}")
message(STATUS "Install:      ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Shared libs:  ${BUILD_SHARED_LIBS}")
message(STATUS "Tests:        ${ENABLE_TESTS}")
message(STATUS "Coverage:     ${ENABLE_COVERAGE}")
message(STATUS "Doc:          ${BUILD_DOC}")
message(STATUS "-------------------------------------------------------------")
message(STATUS)
