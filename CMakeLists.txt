cmake_minimum_required(VERSION 3.18)
project(loadavgmonitord)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-O3 -flto -funroll-loops -g3)
link_libraries(sqlite3)

include_directories(src)
add_subdirectory(src)
install(
    TARGETS
        loadavgmonitord
    DESTINATION
        bin)
