#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Config load_config() {
    Config cfg;
    // дефолтні значення
    strcpy(cfg.theme_bg, "black");
    strcpy(cfg.theme_text, "white");
    cfg.timer_default = 60;

    FILE *f = fopen(CONFIG_FILE, "r");
    if (!f) return cfg; // повертаємо дефолт

    char line[128];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0'; // прибрати \n
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
        if (!key || !value) continue;

        if (strcmp(key, "theme_bg") == 0) strcpy(cfg.theme_bg, value);
        else if (strcmp(key, "theme_text") == 0) strcpy(cfg.theme_text, value);
        else if (strcmp(key, "timer_default") == 0) cfg.timer_default = atoi(value);
    }

    fclose(f);
    return cfg;
}

int save_config(Config cfg) {
    FILE *f = fopen(CONFIG_FILE, "w");
    if (!f) return 0;

    fprintf(f, "theme_bg=%s\n", cfg.theme_bg);
    fprintf(f, "theme_text=%s\n", cfg.theme_text);
    fprintf(f, "timer_default=%d\n", cfg.timer_default);

    fclose(f);
    return 1;
}

int get_color_code(const char *color) {
    if (!color) return 7; // default white
    if (strcmp(color, "black") == 0) return 0;
    if (strcmp(color, "blue") == 0) return 1;
    if (strcmp(color, "green") == 0) return 2;
    if (strcmp(color, "cyan") == 0) return 3;
    if (strcmp(color, "red") == 0) return 4;
    if (strcmp(color, "magenta") == 0) return 5;
    if (strcmp(color, "yellow") == 0) return 6;
    if (strcmp(color, "white") == 0) return 7;
    if (strcmp(color, "gray") == 0) return 8;
    if (strcmp(color, "lightblue") == 0) return 9;
    if (strcmp(color, "lightgreen") == 0) return 10;
    if (strcmp(color, "lightcyan") == 0) return 11;
    if (strcmp(color, "lightred") == 0) return 12;
    if (strcmp(color, "lightmagenta") == 0) return 13;
    if (strcmp(color, "lightyellow") == 0) return 14;
    if (strcmp(color, "brightwhite") == 0) return 15;
    return 7;
}
