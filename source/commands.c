#include <stdio.h>
#include "commands.h"

// Реалізації функцій
int help(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; ++i) {
        printf(" - %-10s : %s\n", commands[i].name, commands[i].help);
    }
    return 0;
}

int echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) printf("%s ", argv[i]);
    printf("\n");
    return 0;
}

int timer(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("Timer placeholder\n");
    return 0;
}

// **Глобальне визначення** масиву — саме тут має бути визначення!
command_t commands[] = {
    {"echo", echo, "Print arguments to screen"},
    {"help", help, "Show this help message"},
    {"timer", timer, "Start timer"},
    {NULL, NULL, NULL}
};
