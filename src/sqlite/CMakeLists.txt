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

if (CPPDBC_FOR_SQLITE)
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

  target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

  find_package(SQLite3 REQUIRED)
  target_link_libraries(${LIBRARY_NAME} PRIVATE SQLite::SQLite3)
endif ()

# =============================================================================
# Installation
# =============================================================================

if (CPPDBC_INSTALL AND CPPDBC_FOR_SQLITE)
  include(GNUInstallDirs)
  install(TARGETS ${LIBRARY_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    EXPORT ${LIBRARY_NAME}-targets)

  install(EXPORT ${LIBRARY_NAME}-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME})

  export(TARGETS ${LIBRARY_NAME}
    FILE ${CMAKE_BINARY_DIR}/${LIBRARY_NAME}-exports.cmake)

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${LIBRARY_NAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME})

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}-config.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME})
endif ()
