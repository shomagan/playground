set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
option(DCMAKE_SH "CMAKE_SH-NOTFOUND" ON)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(COMMON_FLAGS "-ffunction-sections -fdata-sections -g -O0 -fno-common -fmessage-length=0 ")


set(COMPILER_DIRECT C:/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin)
set(CMAKE_C_COMPILER ${COMPILER_DIRECT}/i686-w64-mingw32-gcc.exe)
set(CMAKE_CXX_COMPILER ${COMPILER_DIRECT}/i686-w64-mingw32-g++.exe)
set(CMAKE_ASM_COMPILER ${COMPILER_DIRECT}/i686-w64-mingw32-g++.exe)
set(CMAKE_OBJCOPY ${COMPILER_DIRECT}/objcopy.exe CACHE INTERNAL "GCC TOOLCHAIN OBJCOPY")
set(CMAKE_OBJDUMP ${COMPILER_DIRECT}/objdump.exe CACHE INTERNAL "GCC TOOLCHAIN OBJDUMP")
set(CMAKE_SIZE ${COMPILER_DIRECT}/size.exe CACHE INTERNAL "GCC TOOLCHAIN SIZE")

set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -T${MCU_LINKER_SCRIPT} ")
