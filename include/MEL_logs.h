#ifndef LOGS_H
#define LOGS_H

typedef enum {
	LOG_WARNING, LOG_SUCCESS, LOG_ERROR, LOG_INFORMATION
} logging;

#if __WIN32
#include <windows.h>

void logs(HANDLE hConsole, WORD saved_attributes, int l, const char  * format, ...);

#define TERM_GREEN FOREGROUND_GREEN
#define TERM_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define TERM_RED FOREGROUND_RED
#define TERM_BLUE FOREGROUND_BLUE

#define set_term_color(CONSOLE, TERM_COLOR){\
	SetConsoleTextAttribute(CONSOLE, TERM_COLOR);\
}

#define clear_term_color(CONSOLE, ATR){\
    SetConsoleTextAttribute(CONSOLE, ATR);\
}

#define log_log(...){logs(hConsole, saved_attributes, ##__VA_ARGS__);}

#else
void logs(int l, const char  * format, ...);

#define TERM_GREEN "\033[32m"
#define TERM_YELLOW "\033[33m"
#define TERM_RED "\033[31m"
#define TERM_BLUE "\033[34m"

#define set_term_color(TERM_COLOR){\
	printf(TERM_COLOR);\
}

#define clear_term_color(){\
    printf("\033[0m");\
}

#define log_log logs

#endif

#endif