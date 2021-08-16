#define _GNU_SOURCE

#include "logging.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define MAX_BUFF 100

void _log(const char *__restrict__ _log_file , const char *__restrict__ _level, const char *_file, int _line, char *__restrict__ _format, ...)
{
    time_t _t;
    va_list va;
    va_start(va, _format);
    char *date = calloc(1, MAX_BUFF);
    char *format;
    time(&_t);
    struct tm *datetime = localtime(&_t);
    
    // pretty messy ... but works ;)
    size_t _len = strftime(date, MAX_BUFF, "%x %H:%M:%S ", datetime);
    (!strcmp(_level, ERROR)) ? asprintf(&format, "%s %s %s:%d - %s", date, _level, _file, _line, _format) : 
                               asprintf(&format, "%s %s - %s", date, _level, _format);

    FILE *fp = fopen(_log_file, "a+");
    vfprintf(fp, format, va);
    fclose(fp);
    free(date);
    free(format);
}