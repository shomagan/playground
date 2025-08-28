#sudo apt-get update && sudo apt-get install -y libxml2-dev pkg-config cmake build-essential
#mkdir build
#cmake ..
#cmake --build . --target all;cmake --build . --target copy;cmake --build . --target test
#
cmake_minimum_required(VERSION 3.10)
project(XmlParser CXX)
set(PRJ_NAME xmlParser)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libxml2
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

# Include directories
include_directories(${LIBXML2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/xmlDict.hpp)

# Compiler flags
add_compile_options(${LIBXML2_CFLAGS_OTHER})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(${PRJ_NAME} demo.cpp)
target_link_libraries(${PRJ_NAME} ${LIBXML2_LIBRARIES})

# Custom target for installing dependencies
add_custom_target(install-deps
    COMMAND sudo apt-get update && sudo apt-get install -y libxml2-dev pkg-config
    COMMENT "Installing libxml2 development dependencies"
)

# Test target
add_custom_target(test
    COMMAND ./${PRJ_NAME}
    DEPENDS ${PRJ_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Python-like XML access demo"
)
# copy target
add_custom_target(copy
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/${PRJ_NAME} ${CMAKE_SOURCE_DIR}/
    DEPENDS ${PRJ_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "copy executable file"
)
