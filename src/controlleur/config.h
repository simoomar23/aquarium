#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int controller_port;
    int display_timeout_value;
    int fish_update_interval;
} Config;

int load_config(const char *filename, Config *config);

// Server used functions
int get_controller_port(const Config *config);
int get_display_timeout_value(const Config *config);
int fish_update_interval(const Config *config);
#endif