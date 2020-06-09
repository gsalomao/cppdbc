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

find_package(GTest)

set(TEST_REPORT_PATH "${CMAKE_BINARY_DIR}/test-results" CACHE STRING
  "Folder to store the test reports")

message(STATUS "Test report path: ${TEST_REPORT_PATH}")

add_custom_target(build_tests)

if (ENABLE_COVERAGE)
  message(STATUS "Adding test coverage")
  find_program(LCOV lcov REQUIRED)

  add_custom_target(coverage)
  add_custom_command(TARGET coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${LCOV} -q --rc lcov_function_coverage=0 --directory ${CMAKE_SOURCE_DIR} --capture
    --output-file coverage/coverage.info
    COMMAND ${LCOV} -q --rc lcov_function_coverage=0 --remove coverage/coverage.info
    '*/test/*' '*gtest*' '*gmock*' '/usr/*' --output-file coverage/coverage.info
    COMMAND ${LCOV} --summary coverage/coverage.info
    COMMAND genhtml -q coverage/coverage.info --no-function-coverage --output coverage/html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

add_subdirectory(unit)
add_subdirectory(integration)
