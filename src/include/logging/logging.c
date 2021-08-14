#include "logging.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define MAX_BUFF 1024

void _log(const char *__restrict__ _log_file , const char *__restrict__ _level, const char *_file, int _line, char *__restrict__ _format, ...)
{
    time_t _t;
    va_list va;
    va_start(va, _format);
    char *format = calloc(1, MAX_BUFF);
    time(&_t);
    struct tm *datetime = localtime(&_t);
    
    // pretty messy ... but works ;)
    size_t _len = strftime(format, MAX_BUFF, "%x %H:%M:%S ", datetime);
    (!strcmp(_level, ERROR)) ? snprintf(format+_len, MAX_BUFF, "%s %s:%d - %s", _level, _file, _line, _format) : 
                               snprintf(format+_len, MAX_BUFF, "%s - %s", _level, _format);

    FILE *fp = fopen(_log_file, "a+");
    vfprintf(fp, format, va);
    fclose(fp);
    free(format);
}