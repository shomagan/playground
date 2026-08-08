#sudo apt-get update && sudo apt-get install -y libxml2-dev pkg-config cmake build-essential
#mkdir build
#cmake ..
#cmake --build . --target all;cmake --build . --target copy;cmake --build . --target test
#
cmake_minimum_required(VERSION 3.10)
project(bell202_response CXX C ASM)
set(PRJ_NAME bell202_response)

# Set language standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(BELL202_DATA_SOURCES bell202data.c)

add_library(bell202data ${BELL202_DATA_SOURCES} bell202data.h)
target_compile_features(bell202data PUBLIC c_std_99)
target_compile_options(bell202data PRIVATE -fvisibility=hidden)

add_executable(${PRJ_NAME}
    oracle.cpp
    bell202response.hpp
    bell202data.h
    modbus.c
    modbus.h
    regs_description.c
    regs_description.h
)
target_compile_features(${PRJ_NAME} PRIVATE cxx_std_17)
target_compile_options(${PRJ_NAME} PRIVATE -fvisibility=hidden)
target_link_libraries(${PRJ_NAME} PRIVATE bell202data)
target_link_libraries(bell202data PUBLIC m)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(${PRJ_NAME} PRIVATE -Wl,--strip-all)
endif()

include(GNUInstallDirs)

install(TARGETS bell202data ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# copy target
add_custom_target(copy
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/${PRJ_NAME} ${CMAKE_SOURCE_DIR}/
    DEPENDS ${PRJ_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "copy executable file"
)
add_dependencies(${PRJ_NAME} bell202data)
