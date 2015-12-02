set(TARGET lokilib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin")

file(GLOB SOURCES *.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../include/${TARGET}/loki/*.h)
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(${TARGET} ${SOURCES})
