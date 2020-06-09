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

add_executable(${PROJECT_NAME}_unit EXCLUDE_FROM_ALL)

set(UNIT_TEST_REPORT_FILE "unit-test-report.xml" CACHE STRING
  "File name of the unit test report")

message(STATUS "Unit test report file: ${UNIT_TEST_REPORT_FILE}")

find_program(CTEST ctest REQUIRED)

add_custom_target(test_unit)
add_custom_command(TARGET test_unit COMMAND ${CTEST} -V -R unit-tests)
add_dependencies(build_tests ${PROJECT_NAME}_unit)

add_test(NAME unit-tests
  COMMAND $<TARGET_FILE:${PROJECT_NAME}_unit> --gtest_color=yes
  --gtest_output=xml:${TEST_REPORT_PATH}/${UNIT_TEST_REPORT_FILE})

set_tests_properties(unit-tests PROPERTIES TIMEOUT 10)

file(GLOB_RECURSE COVERAGE_FILES "${CMAKE_BINARY_DIR}/*.gcda" "${CMAKE_BINARY_DIR}/*.gcdO")

if (NOT COVERAGE_FILES STREQUAL "")
  file(REMOVE ${COVERAGE_FILES})
endif ()

target_include_directories(${PROJECT_NAME}_unit PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}_unit
  GTest::GTest
  GMock::GMock
  GMock::Main)

add_subdirectory(sqlite)
