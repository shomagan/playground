cmake_minimum_required(VERSION 3.6)
add_definitions(-DCMAKE_LIBRARY_ARCHITECTURE=x64)

set(PRJ_NAME hex)
add_definitions(-DTERMINAL_GAME=1)
include(gccToolchain.cmake)
project(${PRJ_NAME} C CXX ASM)
include_directories(${CMAKE_SOURCE_DIR})
set(COMMON_SOURCES ${CMAKE_SOURCE_DIR}/hex.cpp)
set(SOURCES "")
set(COMMON_SOURCES ${SOURCES} ${COMMON_SOURCES} ) 
file(GLOB COMMON_HEADERS  ${CMAKE_SOURCE_DIR}/*.hpp)
add_executable (${PRJ_NAME}.exe ${COMMON_SOURCES} ${COMMON_HEADERS} ${FB_HEADERS})
#add_custom_command(TARGET ${PRJ_NAME}.exe POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake_link/${PRJ_NAME}.exe ${CMAKE_SOURCE_DIR}/)


