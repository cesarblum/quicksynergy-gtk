project(QuickSynergy)

cmake_minimum_required(VERSION 2.8)

find_package(GTK2 2.6 REQUIRED gtk)
find_package(Gettext REQUIRED)

set(QUICKSYNERGY_VERSION \"0.9.2\")
set(QUICKSYNERGY_PACKAGE \"QuickSynergy\")
set(ENABLE_NLS 1)
set(LOCALEDIR \"${CMAKE_INSTALL_PREFIX}/locale\")

configure_file(${PROJECT_SOURCE_DIR}/config.h.cmake ${PROJECT_BINARY_DIR}/config.h)

add_subdirectory(po)
add_subdirectory(src)
