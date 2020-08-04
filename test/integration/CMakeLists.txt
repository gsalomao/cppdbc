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

add_executable(${PROJECT_NAME}_integration)

set(INTEGRATION_TEST_REPORT_FILE "integration-test-report.xml" CACHE STRING
  "File name of the integration test report")

message(STATUS "Integration test report file: ${INTEGRATION_TEST_REPORT_FILE}")

find_program(CTEST ctest REQUIRED)

add_custom_target(test_integration)
add_custom_command(TARGET test_integration COMMAND ${CTEST} -V -R integration-tests)

add_test(NAME integration-tests
  COMMAND $<TARGET_FILE:${PROJECT_NAME}_integration> --gtest_color=yes
  --gtest_output=xml:${TEST_REPORT_PATH}/${INTEGRATION_TEST_REPORT_FILE})

set_tests_properties(integration-tests
  PROPERTIES TIMEOUT 600)

find_package(SQLite3 REQUIRED)

target_link_libraries(${PROJECT_NAME}_integration
  GTest::GTest
  GMock::GMock
  GMock::Main
  SQLite::SQLite3)

add_subdirectory(sqlite)
