set(TARGET lokilib)

file(GLOB SOURCES *.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../include/${TARGET}/*.h)
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(${TARGET} ${SOURCES})
