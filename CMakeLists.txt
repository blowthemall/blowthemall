# Project BlowThemALL
project(blowthemall)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

# Metadata
set(BLOWTHEMALL_VERSION_MAJOR 0)
set(BLOWTHEMALL_VERSION_MINOR 0)
set(BLOWTHEMALL_VERSION_PATCH 1)
set(BLOWTHEMALL_VERSION ${BLOWTHEMALL_VERSION_MAJOR}.${BLOWTHEMALL_VERSION_MINOR}.${BLOWTHEMALL_VERSION_PATCH})
set(BLOWTHEMALL_BRIEF "A multiplayer oriented bomberman-like game")

# Dependencies
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
find_package(SDL2 REQUIRED)
find_package(Boost 1.55 COMPONENTS
  date_time
  filesystem
  system
  unit_test_framework
  context
  coroutine
  REQUIRED)

enable_testing()

# Targets
add_subdirectory(src)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# CPack installer
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${BLOWTHEMALL_BRIEF}")
set(CPACK_PACKAGE_VERSION_MAJOR "${BLOWTHEMALL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${BLOWTHEMALL_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${BLOWTHEMALL_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.LESSER")
include(CPack)
