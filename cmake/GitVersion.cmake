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

# Define invalid project version
set(PRJ_VERSION_MAJOR 0)
set(PRJ_VERSION_MINOR 0)
set(PRJ_VERSION_PATCH 0)

# Get Git revision
find_package(Git REQUIRED QUIET)
execute_process(
  COMMAND ${GIT_EXECUTABLE} describe --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_TAG
  ERROR_VARIABLE GIT_TAG_NOT_FOUND
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_STRIP_TRAILING_WHITESPACE
)

# Extract the project version from Git tag
if(NOT GIT_TAG_NOT_FOUND)
  string(REGEX REPLACE "^([vV]*)([0-9]*)([.][0-9]*[.][0-9]*-?.*)$" "\\2"
    TAG_NUMBER ${GIT_TAG})
  set(PRJ_VERSION_MAJOR ${TAG_NUMBER})

  string(REGEX REPLACE "^([vV]*)([0-9]*[.])([0-9]*)([.][0-9]*-?.*)$" "\\3"
    TAG_NUMBER ${GIT_TAG})
  set(PRJ_VERSION_MINOR ${TAG_NUMBER})

  string(REGEX REPLACE "^([vV]*)([0-9]*[.][0-9]*[.])([0-9]*)(-?.*)$" "\\3"
    TAG_NUMBER ${GIT_TAG})
  set(PRJ_VERSION_PATCH ${TAG_NUMBER})
endif()

# Define semantic project version
set(PROJECT_VERSION_MAJOR ${PRJ_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${PRJ_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${PRJ_VERSION_PATCH})
set(PROJECT_VERSION
  "${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
