#include "log_file.h"
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

static FILE *log_file = NULL;

const char* log_level_str(LogLevel level) {
    switch (level) {
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default:        return "UNKNOWN";
    }
}

int init_log(const char *filename) {
    log_file = fopen(filename, "a");
    if (!log_file) {
        return -1;
    }
    return 0;
}

void close_log() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_message(LogLevel level, const char *format, ...) {
    if (!log_file) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S]", t);

    fprintf(log_file, "%s [%s] ", time_str, log_level_str(level));

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
}
