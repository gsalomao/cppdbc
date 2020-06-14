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
# Library
# =============================================================================

set(DATABASE sqlite)
set(LIBRARY_NAME ${PROJECT_NAME}-${DATABASE})

add_library(${LIBRARY_NAME})
add_library(${PROJECT_NAME}::${DATABASE} ALIAS ${LIBRARY_NAME})

set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  EXPORT_NAME ${PROJECT_NAME}::${DATABASE})

target_sources(${LIBRARY_NAME} PRIVATE
  sqlite_database.cpp
  sqlite_resultset.cpp
  sqlite_statement.cpp
  sqlite_transaction.cpp)

# =============================================================================
# Dependencies
# =============================================================================

find_package(SQLite3 REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE SQLite::SQLite3)

# =============================================================================
# Installation
# =============================================================================

install(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
