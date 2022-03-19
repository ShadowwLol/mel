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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linking

message(STATUS ${CMAKE_C_COMPILER})

if(LINUX)
	message(STATUS "Current Platform: LINUX")

if (DEBUG)
	target_link_libraries(${PROJECT_NAME} -l:libdl.so -l:libglfw.so -l:libpthread.so -l:libGL.so -l:libm.so)
else()
	target_link_libraries(${PROJECT_NAME} -ldl -l:libglfw3.a -lpthread -l:libGL.a -l:libm.so)
endif()

else()
	message(STATUS "Current Platform: WINDOWS")

if (DEBUG)
	link_directories(/usr/i686-w64-mingw32/bin /usr/i686-w64-mingw32/bin)
	target_link_libraries(${PROJECT_NAME} libglfw3dll.a libopengl32.a libmingw32.a libgdi32.a libuser32.a libkernel32.a libpthread.a libm.a)
else()
	link_directories(/usr/i686-w64-mingw32/lib /usr/i686-w64-mingw32/lib)
	target_link_libraries(${PROJECT_NAME} libglfw3.a libopengl32.a libmingw32.a libgdi32.a libuser32.a libkernel32.a libwinpthread.a libwinpthread.dll.a libssp.a libm.a)
endif()

endif(LINUX)
