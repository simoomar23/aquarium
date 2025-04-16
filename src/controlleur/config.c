#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

// Helper: Trim leading and trailing whitespace
void trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // All spaces?
    if (*str == 0) {
        *str = '\0';
        return;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';
}

// Main config loader
int load_config(const char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open config file");
        return -1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        char key[128] = {0};
        char value[128] = {0};

        if (sscanf(line, " %127[^=]=%127[^\n\r]", key, value) == 2) {
            trim_whitespace(key);
            trim_whitespace(value);

            if (strcmp(key, "controller-port") == 0) {
                config->controller_port = atoi(value);
            } else if (strcmp(key, "display-timeout-value") == 0) {
                config->display_timeout_value = atoi(value);
            } else if (strcmp(key, "fish-update-interval") == 0) {
                config->fish_update_interval = atoi(value);
            }
        }

        // Invalid lines are ignored
    }

    fclose(file);
    return 0;
}

// Server used functions
int get_controller_port(const Config *config){
    return config->controller_port;
}
int get_display_timeout_value(const Config *config){
    return config->display_timeout_value;
}
int fish_update_interval(const Config *config){
    return config->fish_update_interval;
}