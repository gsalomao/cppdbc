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

################################################################################
# The following targets will be created:
#   - gtest               - Target for GTest library
#   - gmock               - Target for GMock library
#   - gtest_main          - Target for GTest library with main function
#   - gmock_main          - Target for GMock library with main function
#   - GTest::GTest        - Target for GTest library
#   - GMock::GMock        - Target for GMock library
#   - GTest::Main         - Target for GTest library with main function
#   - GMock::Main         - Target for GMock library with main function
#
# The following variables will be set:
#   - GTest_FOUND         - Indicate if the package has been found
#   - GMock_FOUND         - Indicate if the package has been found
################################################################################

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/vendor/deps)

fetchcontent_declare(gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.10.0
  )

fetchcontent_getproperties(gtest)

if (NOT gtest_POPULATED)
  message(STATUS "Fetching Google Test")

  fetchcontent_populate(gtest)
  add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR} EXCLUDE_FROM_ALL)

  set(GTest_FOUND true)

  if (NOT TARGET GTest::GTest)
    add_library(GTest::GTest ALIAS gtest)
  endif ()

  if (NOT TARGET GTest::Main)
    add_library(GTest::Main ALIAS gtest_main)
  endif ()

  set(GMock_FOUND true)

  if (NOT TARGET GMock::GMock)
    add_library(GMock::GMock ALIAS gmock)
  endif ()

  if (NOT TARGET GMock::Main)
    add_library(GMock::Main ALIAS gmock_main)
  endif ()
endif ()
