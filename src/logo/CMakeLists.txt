add_custom_target(qslogo.h ALL
                  COMMAND gdk-pixbuf-csource --raw --extern --build-list qslogo ${CMAKE_CURRENT_SOURCE_DIR}/qslogo.png > ${CMAKE_CURRENT_BINARY_DIR}/qslogo.h
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/qslogo.png)
