cmake_minimum_required(VERSION 3.6)
add_definitions(-DCMAKE_LIBRARY_ARCHITECTURE=x64)

set(GDIPLUS_LIBRARY gdiplus)
set(SHLWAPI_LIBRARY shlwapi)

set(PRJ_NAME fb_test)
add_definitions(-DSOFI_LINK=1)
include(gccToolchain.cmake)
project(${PRJ_NAME} C CXX ASM)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/fb)
set(COMMON_SOURCES ${CMAKE_SOURCE_DIR}/main.c)
set(SOURCES "")
set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/fb)
add_subdirectory(fb)
set(COMMON_SOURCES ${SOURCES} ${COMMON_SOURCES} ) 
file(GLOB COMMON_HEADERS  ${CMAKE_SOURCE_DIR}/*.h)
file(GLOB FB_HEADERS  ${CMAKE_SOURCE_DIR}/fb/*.h)
add_executable (${PRJ_NAME}.exe ${COMMON_SOURCES} ${COMMON_HEADERS} ${FB_HEADERS})
target_link_libraries(${PRJ_NAME}.exe ${GDIPLUS_LIBRARY} ${SHLWAPI_LIBRARY})
#add_custom_command(TARGET ${PRJ_NAME}.exe POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake_link/${PRJ_NAME}.exe ${CMAKE_SOURCE_DIR}/)


