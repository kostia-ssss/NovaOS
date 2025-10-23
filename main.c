#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

#define INPUT_SIZE 256

int main() {
    printf("Welcome to NovaOS!\n");
    char input[INPUT_SIZE];

    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0') continue;

        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");

        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) help();
        else if (strcmp(cmd, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        else if (strcmp(cmd, "timer") == 0) {
            if (!arg1) {
                printf("Error: specify seconds. Example: timer 5\n");
                continue;
            }
            char *endptr;
            long val = strtol(arg1, &endptr, 10);
            if (*endptr != '\0' || val <= 0) {
                printf("Error: invalid argument '%s'\n", arg1);
                continue;
            }
            timer((int)val);
        }
        else if (strcmp(cmd, "clear") == 0) clear();
        else if (strcmp(cmd, "cc") == 0) change_color(arg1);
        else if (strcmp(cmd, "doc") == 0) doc(arg1);
        else printf("Unknown command: %s\n", cmd);
    }

    return 0;
}
