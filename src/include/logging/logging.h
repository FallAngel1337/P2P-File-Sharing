/**
 * logging.h - generic logging library
 * Author: FallAngel
 * Date: 2021-08-14
 * 
 * Copyright (c) 2021
 * 
 * original code: https://github.com/FallAngel1337/logginglib
 */

#ifndef _LOGGING_H
#define _LOGGING_H

#define __LOG_ERROR__ "/var/log/p2pd/error.log"
#define __LOG_INFO__ "/var/log/p2pd/info.log"

#define LOG_ERROR(...)\
            (_log(__LOG_ERROR__, (ERROR), __FILE__, __LINE__, __VA_ARGS__))

#define LOG_INFO(...)\
            (_log(__LOG_INFO__, (INFO), __FILE__, __LINE__, __VA_ARGS__))

#define LOG_WARN(...)\
            (_log(__LOG_INFO__, (WARN), __FILE__, __LINE__, __VA_ARGS__))

// Log levels/priority
#define ERROR   "[ ERROR ]"
#define WARNING "[ WARN ]"
#define INFO    "[ INFO ]"

void _log(const char *__restrict__ _log_file, const char *__restrict__ _level, const char *_file, int _line, char *__restrict__ _format, ...);

#endif /* _LOGGING_H */