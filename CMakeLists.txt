# Project BlowThemALL
project(blowthemall)

# Metadata
set(BLOWTHEMALL_VERSION_MAJOR 0)
set(BLOWTHEMALL_VERSION_MINOR 0)
set(BLOWTHEMALL_VERSION_PATCH 0)
set(BLOWTHEMALL_VERSION ${BLOWTHEMALL_VERSION_MAJOR}.${BLOWTHEMALL_VERSION_MINOR}.${BLOWTHEMALL_VERSION_PATCH})
set(BLOWTHEMALL_BRIEF "A multiplayer oriented bomberman-like game")

# Dependencies
cmake_minimum_required(VERSION 3.1.0)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Tufao1 REQUIRED)

include_directories(${TUFAO_INCLUDE_DIR})

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
