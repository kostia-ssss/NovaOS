#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // для Sleep на Windows

void show_help() {
    printf("Available commands:\n");
    printf("  help         - Show this help message\n");
    printf("  timer <sec>  - Countdown timer\n");
    printf("  exit         - Exit NovaOS\n");
    printf("  echo <text>  - Print text\n");
    printf("  calc <expr>  - Simple calculator (e.g., 3+5)\n");
    printf("  sysinfo      - Show system info\n");
}

void run_timer(int seconds) {
    for (int i = seconds; i > 0; i--) {
        printf("%d\n", i);
        Sleep(1000); // 1 секунда
    }
    printf("Time's up!\n");
}

void echo(const char *text) {
    if (text)
        printf("%s\n", text);
    else
        printf("\n");
}

void calc(const char *expr) {
    if (!expr) {
        printf("Usage: calc <a+b> or <a-b>\n");
        return;
    }

    int a, b;
    char op;
    if (sscanf(expr, "%d%c%d", &a, &op, &b) == 3) {
        switch (op) {
            case '+': printf("%d\n", a + b); break;
            case '-': printf("%d\n", a - b); break;
            case '*': printf("%d\n", a * b); break;
            case '/':
                if (b == 0) printf("Error: divide by zero\n");
                else printf("%d\n", a / b);
                break;
            default:
                printf("Unknown operator: %c\n", op);
        }
    } else {
        printf("Invalid expression. Try like: calc 3+5\n");
    }
}

void sysinfo() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    printf("System time: %02d:%02d:%02d\n", t.wHour, t.wMinute, t.wSecond);
    printf("Date: %02d/%02d/%04d\n", t.wDay, t.wMonth, t.wYear);
    printf("OS: Windows (NovaOS shell)\n");
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
        }
        else if (strcmp(cmd, "echo") == 0) {
            echo(arg);
        }
        else if (strcmp(cmd, "calc") == 0) {
            calc(arg);
        }
        else if (strcmp(cmd, "sysinfo") == 0) {
            sysinfo();
        }
        else if (strcmp(cmd, "exit") == 0) {
            printf("Shutting down NovaOS...\n");
            break;
        } else {
            printf("Unknown command. Type 'help'.\n");
        }
    }

    return 0;
}
