set(TARGET loki_test)

file(GLOB SOURCES *.cpp *.h)
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} loki)

add_dependencies(${TARGET} loki)

add_test(NAME loki_test COMMAND ${TARGET})