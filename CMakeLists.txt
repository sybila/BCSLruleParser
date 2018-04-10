cmake_minimum_required(VERSION 3.0)

project(RuleParser)

include_directories(src)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "-std=c++11")
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_subdirectory(js)
add_subdirectory(main)
add_subdirectory(src)
add_subdirectory(swig)
