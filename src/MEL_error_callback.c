#include "../include/MEL_logs.h"

#if __WIN32
#include <windows.h>
extern HANDLE hConsole;
extern WORD saved_attributes;
#endif

void error_callback(int error, const char* description){ log_log(LOG_ERROR, description); }
