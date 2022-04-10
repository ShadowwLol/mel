#ifndef _MEL_LOGS_H
#define _MEL_LOGS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
	LOG_WARNING, LOG_SUCCESS, LOG_ERROR, LOG_INFORMATION
} logs;

#if __WIN32
#include <windef.h>
#include <winbase.h>
#include <wincon.h>

#define TERM_GREEN FOREGROUND_GREEN
#define TERM_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define TERM_RED FOREGROUND_RED
#define TERM_BLUE FOREGROUND_BLUE

static bool dirty = false;
static HANDLE hConsole;
static WORD saved_attributes;

static void MEL_set_color(const WORD TERM_COLOR);
inline static void MEL_set_color(const WORD TERM_COLOR){
	if (!dirty){
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		saved_attributes = consoleInfo.wAttributes;
		dirty = true;
	}

	SetConsoleTextAttribute(hConsole, TERM_COLOR);
}

static void MEL_clear_color();
inline static void MEL_clear_color(){
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

#else
#include <unistd.h>

#define TERM_GREEN "\033[32m"
#define TERM_YELLOW "\033[33m"
#define TERM_RED "\033[31m"
#define TERM_BLUE "\033[34m"

static void MEL_set_color(const char * TERM_COLOR);
inline static void MEL_set_color(const char * TERM_COLOR){
	if (isatty(STDOUT_FILENO)){
		printf("%s", TERM_COLOR);
	}
}

static void MEL_clear_color();
inline static void MEL_clear_color(){
	if (isatty(STDOUT_FILENO)){
		printf("\033[0m");
	}
}

#endif

static void MEL_log(logs, const char *, ...);

inline static void MEL_log(logs l, const char * format, ...){
	switch(l){
		case LOG_WARNING:
			MEL_set_color(TERM_YELLOW);
			fprintf(stdout, "[!] ");
			break;
		case LOG_SUCCESS:
			MEL_set_color(TERM_GREEN);
			fprintf(stdout, "[+] ");
			break;
		case LOG_ERROR:
			MEL_set_color(TERM_RED);
			fprintf(stdout, "[-] ");
			break;
		case LOG_INFORMATION:
			MEL_set_color(TERM_BLUE);
			fprintf(stdout, "[#] ");
			break;
		default:
			break;
	}
	MEL_clear_color();

	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	putchar('\n');
	va_end(args);
}

#endif
