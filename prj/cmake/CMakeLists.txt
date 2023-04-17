cmake_minimum_required(VERSION 3.7)
project(AntiDuplX)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include_directories(${ROOT_DIR}/src)
include_directories(${ROOT_DIR}/3rd/Cpl/src)

file(GLOB_RECURSE ADX_SRC ${ROOT_DIR}/src/AntiDuplX/*.cpp)
add_executable(AntiDuplX ${ADX_SRC})
target_link_libraries(AntiDuplX -ldl -lpthread -lstdc++)

