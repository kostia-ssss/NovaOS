#ifndef UTILS_H
#define UTILS_H

#define CONFIG_FILE "data/config.cfg"

typedef struct {
    char theme_bg[32];
    char theme_text[32];
    char user[32];
    int timer_default;
    int password;
} Config;

// Завантажує конфіг з файлу. Якщо файлу нема, заповнює дефолтами.
Config load_config();

// Зберігає конфіг у файл.
int save_config(Config cfg);

// Допоміжна функція для перетворення кольору в код консолі
int get_color_code(const char *color);

#endif
