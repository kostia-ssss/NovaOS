#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "commands.h"
#include "utils.h"

#define INPUT_SIZE 256

// Парсер одного цілого аргументу з дефолтом.
// Повертає 1 — успіх (out_val встановлено), 0 — помилка формату.
int parse_int_default(const char *s, int default_val, int *out_val) {
    if (!s) {
        *out_val = default_val;
        return 1;
    }

    char *endptr = NULL;
    long v = strtol(s, &endptr, 10);

    if (endptr == s) return 0;             // нічого не прочитано
    if (*endptr != '\0') return 0;         // залишилися неприпустимі символи
    if (v < INT_MIN || v > INT_MAX) return 0; // вихід за межі int

    *out_val = (int)v;
    return 1;
}

int main() {
    Config cfg = load_config();
    theme(cfg.theme_bg, cfg.theme_text);
    clear();
    load_config();
    printf("Welcome to NovaOS!\n");
    char input[INPUT_SIZE];

    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0') continue;

        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");
        char *arg3 = strtok(NULL, " ");

        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) {
            help();
        }
        else if (strcmp(cmd, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        else if (strcmp(cmd, "timer") == 0) {
            // Перший аргумент обов'язковий і має бути додатнім числом (>0)
            if (!arg1) {
                printf("Error: specify seconds. Example: timer 5 [arg2] [arg3]\n");
                continue;
            }

            int sec = 0, a2 = 0, a3 = 0;
            if (!parse_int_default(arg1, 0, &sec) || sec <= 0) {
                printf("Error: invalid seconds '%s'\n", arg1);
                continue;
            }
            if (!parse_int_default(arg2, 0, &a2)) {
                printf("Error: invalid second argument '%s'\n", arg2);
                continue;
            }
            if (!parse_int_default(arg3, 0, &a3)) {
                printf("Error: invalid third argument '%s'\n", arg3);
                continue;
            }

            // Виклик функції timer — оновити сигнатуру в commands.h/commands.c якщо потрібно
            timer(sec, a2, a3);
        }
        else if (strcmp(cmd, "clear") == 0) {
            clear();
        }
        else if (strcmp(cmd, "cc") == 0) {
            change_color(arg1);
        }
        else if (strcmp(cmd, "doc") == 0) {
            doc(arg1);
        }
        else if (strcmp(cmd, "theme") == 0) {
            theme(arg1, arg2);
        }
        else if (strcmp(cmd, "echo") == 0) {
            echo(arg1);
        }
        else if (strcmp(cmd, "music") == 0) {
            music();
        }
        else if (strcmp(cmd, "time") == 0) {
            time_();
        }
        else {
            printf("Unknown command: %s\n", cmd);
        }
    }

    return 0;
}
