set(QUICKSYNERGY_SOURCES callbacks.c main.c synergy_config.c ui.c)

add_executable(quicksynergy ${QUICKSYNERGY_SOURCES})

add_dependencies(quicksynergy qslogo.h)

include_directories(${PROJECT_BINARY_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${GTK2_INCLUDE_DIRS})

target_link_libraries(quicksynergy ${GTK2_LIBRARIES})

install(TARGETS quicksynergy DESTINATION bin)

add_subdirectory(logo)
