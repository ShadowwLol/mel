cmake_minimum_required(VERSION 3.10)

# Variables
if(UNIX AND NOT APPLE AND NOT ($ENV{PLATFORM} MATCHES "WINDOWS"))
	set(LINUX TRUE)
	set($ENV{CC} gcc)
else()
	set($ENV{CC} i686-w64-mingw32-gcc)
endif()

# Project Name
project(mel VERSION 1.0)

# Source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directory
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linking

if(LINUX)
	message(STATUS "Current Platform: LINUX")

	find_library(LDL dl REQUIRED)
	find_library(LGLFW glfw REQUIRED)
	find_library(LPTHREAD pthread REQUIRED)
	find_library(LGL GL REQUIRED)
	find_library(LM m REQUIRED)

	target_link_libraries(${PROJECT_NAME} ${LDL} ${LGLFW} ${LPTHREAD} ${LGL} ${LM})
else()
	message(STATUS "Current Platform: UNKNOWN")
	message(STATUS "Building For: WINDOWS")

	find_library(LGLFW3DLL    libglfw3dll.a REQUIRED /usr/i686-w64-mingw32/lib/libglfw3dll.a)
	find_library(LOPENGL32  libopengl32.a REQUIRED /usr/i686-w64-mingw32/lib/libopengl32.a)
	find_library(LMINGW   libmingw32.a  REQUIRED /usr/i686-w64-mingw32/lib/libmingw32.a)
	find_library(LGDI     libgdi32.a    REQUIRED /usr/i686-w64-mingw32/lib/libgdi32.a)
	find_library(LUSER    libuser32.a   REQUIRED /usr/i686-w64-mingw32/lib/libuser32.a)
	find_library(LKERNEL  libkernel32.a REQUIRED /usr/i686-w64-mingw32/lib/libkernel32.a)
	find_library(LPTHREAD libpthread.a  REQUIRED /usr/i686-w64-mingw32/lib/libpthread.a)
	find_library(LM       libm.a        REQUIRED /usr/i686-w64-mingw32/lib/libm.a)

	target_link_libraries(${PROJECT_NAME} ${LGLFW} ${LOPENGL} ${LMINGW} ${LGDI} ${LUSER} ${LKERNEL} ${LPTHREAD} ${LM})
endif(LINUX)
