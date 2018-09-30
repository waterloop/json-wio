cmake_minimum_required(VERSION 3.1.0)
project(json-wio)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os -ffunction-sections -fdata-sections -fno-rtti")

set(WIO_MODULE_DIR ${CMAKE_CURRENT_LIST_DIR}/.wio/node_modules)

set(wlib-stl_dir ${WIO_MODULE_DIR}/wlib-stl__3.0.2)
set(wlib-mem_dir ${WIO_MODULE_DIR}/wlib-memory__1.0.4)
set(wlib-mal_dir ${WIO_MODULE_DIR}/wlib-malloc__1.0.4)
set(wlib-tmp_dir ${WIO_MODULE_DIR}/wlib-tmp__1.0.2)

set(JSON_DIR ${CMAKE_CURRENT_LIST_DIR})
set(GTEST_DIR ${CMAKE_CURRENT_LIST_DIR}/googletest/googletest)

foreach (module wlib-stl)
    set(dir ${${module}_dir})
    file(GLOB_RECURSE ${module}_files ${dir}/src/*.cpp ${dir}/src/**/*.cpp)
    add_library(${module} ${${module}_files})
    target_include_directories(${module} PUBLIC ${dir}/include)
endforeach ()
foreach (module wlib-stl wlib-mal wlib-mem wlib-tmp)
    set(${module}_include ${${module}_dir}/include)
endforeach ()
target_include_directories(wlib-stl PRIVATE
    ${wlib-mem_include}
    ${wlib-tmp_include}
    ${wlib-mal_include}
    )

file(GLOB_RECURSE JSON_FILES
    ${JSON_DIR}/include/wlib/**/*.h
    ${JSON_DIR}/src/wlib/**/*.cpp
    )

add_library(json ${JSON_FILES})
target_link_libraries(json wlib-stl)
target_compile_definitions(json PUBLIC
    WLIB_JSON_LONG_DOUBLE
    WLIB_JSON_LONG_LONG
    )
target_include_directories(json PUBLIC
    ${JSON_DIR}/include
    ${wlib-stl_include}
    ${wlib-tmp_include}
    ${wlib-mem_include}
    ${wlib-mal_include}
    )

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/googletest)

file(GLOB_RECURSE UNIT_FILES
    ${CMAKE_CURRENT_LIST_DIR}/unit/*.cpp
    )

add_executable(unit ${UNIT_FILES})
target_link_libraries(unit json gtest)
target_include_directories(unit PUBLIC
    ${WLIB_DIR}/include
    ${JSON_DIR}/include
    ${GTEST_DIR}/include
    )
