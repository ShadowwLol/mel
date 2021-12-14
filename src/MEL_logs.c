#include "../include/MEL_logs.h"

void MEL_log(int l, const char * format, ...){
    switch(l){
#if __WIN32
        case LOG_WARNING:
            set_term_color(hConsole, TERM_YELLOW);
            fprintf(stdout, "[!] ");
            break;
        case LOG_SUCCESS:
            set_term_color(hConsole, TERM_GREEN);
            fprintf(stdout, "[+] ");
            break;
        case LOG_ERROR:
            set_term_color(hConsole, TERM_RED);
            fprintf(stderr, "[-] ");
            break;
        case LOG_INFORMATION:
            set_term_color(hConsole, TERM_BLUE);
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    clear_term_color(hConsole, saved_attributes);
#else
        case LOG_WARNING:
            set_term_color(TERM_YELLOW);
            fprintf(stdout, "[!] ");
            break;
        case LOG_SUCCESS:
            set_term_color(TERM_GREEN);
            fprintf(stdout, "[+] ");
            break;
        case LOG_ERROR:
            set_term_color(TERM_RED);
            fprintf(stderr, "[-] ");
            break;
        case LOG_INFORMATION:
            set_term_color(TERM_BLUE);
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    clear_term_color();
#endif
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    putchar('\n');
    va_end(args);
}
