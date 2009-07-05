project(QuickSynergy)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

find_package(PkgConfig REQUIRED)
find_package(Gettext REQUIRED)

pkg_check_modules(GTK REQUIRED gtk+-2.0)

set(QUICKSYNERGY_VERSION \"0.9.1\")
set(QUICKSYNERGY_PACKAGE \"QuickSynergy\")
set(ENABLE_NLS 1)
set(LOCALEDIR \"${CMAKE_INSTALL_PREFIX}/locale\")

configure_file(${PROJECT_SOURCE_DIR}/config.h.cmake ${PROJECT_BINARY_DIR}/config.h)

add_subdirectory(po)
add_subdirectory(src)
