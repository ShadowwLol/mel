#include "../include/MEL_logs.h"

#if __WIN32
void logs(HANDLE hc, WORD sa, int l, const char  * format, ...){
    switch(l){
        case LOG_WARNING:
            set_term_color(hc, TERM_YELLOW);
            fprintf(stdout, "[!] ");
            break;
        case LOG_SUCCESS:
            set_term_color(hc, TERM_GREEN);
            fprintf(stdout, "[+] ");
            break;
        case LOG_ERROR:
            set_term_color(hc, TERM_RED);
            fprintf(stderr, "[-] ");
            break;
        case LOG_INFORMATION:
            set_term_color(hc, TERM_BLUE);
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    clear_term_color(hc, sa);
#else
void logs(int l, const char * format, ...){
    switch(l){
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
