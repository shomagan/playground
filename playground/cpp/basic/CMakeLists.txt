cmake_minimum_required(VERSION 3.14)
add_definitions(-DCMAKE_LIBRARY_ARCHITECTURE=x64)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PRJ_NAME listen_server)

include(FetchContent)
FetchContent_Declare(
   nlohmann_json
   GIT_REPOSITORY https://github.com/nlohmann/json.git
   GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

include(gccToolchain.cmake)
project(${PRJ_NAME} C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})

set(COMMON_SOURCES
   ${CMAKE_SOURCE_DIR}/main.cpp
   ${CMAKE_SOURCE_DIR}/config.cpp
   ${CMAKE_SOURCE_DIR}/keyboard.cpp
   ${CMAKE_SOURCE_DIR}/http_client.cpp
   ${CMAKE_SOURCE_DIR}/device_updater.cpp
   ${CMAKE_SOURCE_DIR}/udp_listener.cpp
)

file(GLOB COMMON_HEADERS ${CMAKE_SOURCE_DIR}/*.hpp ${CMAKE_SOURCE_DIR}/*.h)

add_executable(${PRJ_NAME}.exe ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(${PRJ_NAME}.exe nlohmann_json::nlohmann_json)

if(WIN32)
   target_link_libraries(${PRJ_NAME}.exe ws2_32)
endif()
