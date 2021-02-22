configure_file(definitions.h.in ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h)

file(GLOB_RECURSE COLLECTED_SOURCES *.cpp)
file(GLOB_RECURSE COLLECTED_HEADERS *.h)

add_executable(${PROJECT_NAME}
   ${COLLECTED_SOURCES}
   ${COLLECTED_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
   pthread
)

install(TARGETS ${PROJECT_NAME}
   RUNTIME DESTINATION bin
)
