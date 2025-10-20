#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // для Sleep на Windows

void show_help() {
    printf("Available commands:\n");
    printf("  help         - Show this help message\n");
    printf("  timer <sec>  - Countdown timer\n");
    printf("  exit         - Exit NovaOS\n");
}

void run_timer(int seconds) {
    for (int i = seconds; i > 0; i--) {
        printf("%d\n", i);
        Sleep(1000); // 1 секунда
    }
    printf("Time's up!\n");
}

int main() {
    char input[128];

    printf("Welcome to NovaOS shell!\n");
    printf("Type 'help' for commands.\n\n");

    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // прибираємо \n або \r\n
        input[strcspn(input, "\r\n")] = 0;

        // отримуємо команду та аргумент
        char *cmd = strtok(input, " ");
        char *arg = strtok(NULL, "");

        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) {
            show_help();
        } else if (strcmp(cmd, "timer") == 0) {
            int sec = 5; // за замовчуванням
            if (arg) sec = atoi(arg);
            if (sec <= 0) sec = 5;
            run_timer(sec);
        } else if (strcmp(cmd, "exit") == 0) {
            printf("Shutting down NovaOS...\n");
            break;
        } else {
            printf("Unknown command. Type 'help'.\n");
        }
    }

    return 0;
}
