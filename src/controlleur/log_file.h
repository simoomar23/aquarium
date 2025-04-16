#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <stdio.h>

// Log levels
typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
} LogLevel;

// Initializes the log system by opening the log file
int init_log(const char *filename);

// Closes the log file
void close_log();

// Logs a message with the given log level and format string
void log_message(LogLevel level, const char *format, ...);

#endif // LOG_FILE_H
