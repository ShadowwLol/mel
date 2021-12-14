#include "../include/MEL_def.h"
#include "../include/MEL_logs.h"

void error_callback(int error, const char* description){ MEL_log(LOG_ERROR, description); }
