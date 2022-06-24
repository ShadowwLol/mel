#include "inc/def.h"
#include "inc/logs.h"

void error_callback(int error, const char* description){ MEL_log(LOG_ERROR, description); }
