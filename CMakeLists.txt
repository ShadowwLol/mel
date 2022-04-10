cmake_minimum_required(VERSION 3.10)

# Variables
if(UNIX AND NOT APPLE AND NOT ($ENV{PLATFORM} MATCHES "WINDOWS"))
	set(LINUX TRUE)
	set(CMAKE_C_COMPILER "/usr/bin/gcc")
	set($ENV{CC} "/usr/bin/gcc")
else()
	set(CMAKE_C_COMPILER "/usr/bin/i686-w64-mingw32-gcc")
	set($ENV{CC} "/usr/bin/i686-w64-mingw32-gcc")
endif()

# Project Name
project(mel VERSION 1.0)

#${CMAKE_C_FLAGS}

# CFLAGS
if ($ENV{C_DEBUG} MATCHES "YES")
    set(DEBUG TRUE)
    message(STATUS "[#] Mode: DEBUG")
    set(CMAKE_C_FLAGS "-O -g -std=c99 -Wextra -fstack-protector -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -Wshadow -Wformat=2 -Wformat-truncation -Wformat-overflow -fno-common -fstack-usage")
else()
    set(DEBUG FALSE)
    message(STATUS "[#] Mode: RELEASE")
    set(CMAKE_C_FLAGS "-static-libgcc -std=c99 -O3 -Wextra -fstack-protector -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -Wshadow -Wformat=2 -Wformat-truncation -Wformat-overflow -fno-common -fstack-usage")
endif()

# Source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directory
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/inc)

# Linking

message(STATUS ${CMAKE_C_COMPILER})

if(LINUX)
	message(STATUS "Current Platform: LINUX")
	target_link_libraries(${PROJECT_NAME} -ldl -l:libglfw3.a -pthread -l:libGL.a -lm)
else()
	message(STATUS "Current Platform: WINDOWS")
	target_link_libraries(${PROJECT_NAME} libglfw3.a libopengl32.a libmingw32.a libgdi32.a libuser32.a libkernel32.a libwinpthread.a libwinpthread.dll.a libssp.a libm.a)
endif(LINUX)
