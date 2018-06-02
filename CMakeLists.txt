cmake_minimum_required(VERSION 3.1.0)
project(json-wio)

set(CMAKE_CXX_STANDARD 11)

set(WLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/.wio/node_modules/wlib-wio)
set(JSON_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE WLIB_FILES
    ${WLIB_DIR}/include/wlib/**/*.h
    ${WLIB_DIR}/src/wlib/**/*.cpp
    )

add_library(wlib ${WLIB_FILES})
target_include_directories(wlib PUBLIC ${WLIB_DIR}/include)
target_compile_definitions(wlib PUBLIC MAX_ALLOCATORS=10u NUM_BLOCKS=400u)

file(GLOB_RECURSE JSON_FILES
    ${JSON_DIR}/include/wlib/**/*.h
    ${JSON_DIR}/src/wlib/**/*.cpp
    )

add_library(json ${JSON_FILES})
target_link_libraries(json wlib)
target_include_directories(json PUBLIC ${JSON_DIR}/include)
target_include_directories(json PUBLIC ${WLIB_DIR}/include)

file(GLOB_RECURSE UNIT_FILES
    ${CMAKE_CURRENT_LIST_DIR}/unit/*.cpp
    )

add_executable(unit ${UNIT_FILES})
target_link_libraries(unit json wlib)
target_include_directories(unit PUBLIC ${WLIB_DIR}/include)
target_include_directories(unit PUBLIC ${JSON_DIR}/include)
