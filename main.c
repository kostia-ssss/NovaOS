// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // Sleep
#include <math.h>

void show_help();
void run_timer(int seconds);
void echo_text(const char *text);
void calc(const char *expr);
void sysinfo();
void savecmd(const char *text);
void readlog();

int main() {
    char input[256];

    printf("Welcome to NovaOS shell!\n");
    printf("Type 'help' for commands.\n\n");

    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // обрізаємо \r і \n (Windows і Unix)
        input[strcspn(input, "\r\n")] = 0;

        // якщо пустий рядок — пропускаємо
        if (input[0] == '\0') continue;

        // робимо копію повного вводу для логування (тому що strtok змінює рядок)
        char fullcmd[256];
        strncpy(fullcmd, input, sizeof(fullcmd)-1);
        fullcmd[sizeof(fullcmd)-1] = '\0';

        // розбираємо команду та аргумент (все після першого пробілу як arg)
        char *cmd = strtok(input, " ");
        char *arg = strtok(NULL, ""); // все після першого пробілу

        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) {
            show_help();
            savecmd(fullcmd);
        }
        else if (strcmp(cmd, "timer") == 0) {
            int sec = 5; // дефолт
            if (arg) sec = atoi(arg);
            if (sec <= 0) sec = 5;
            run_timer(sec);
            savecmd(fullcmd);
        }
        else if (strcmp(cmd, "echo") == 0) {
            echo_text(arg);
            savecmd(fullcmd);
        }
        else if (strcmp(cmd, "calc") == 0) {
            calc(arg);
            savecmd(fullcmd);
        }
        else if (strcmp(cmd, "sysinfo") == 0) {
            sysinfo();
            savecmd(fullcmd);
        }
        else if (strcmp(cmd, "log") == 0) {
            readlog();
            savecmd(fullcmd);
        }
        else if (strcmp(cmd, "exit") == 0) {
            printf("Shutting down NovaOS...\n");
            savecmd(fullcmd);
            break;
        }
        else {
            printf("Unknown command. Type 'help'.\n");
        }
    }

    return 0;
}

// --------------------------
void show_help() {
    printf("Available commands:\n");
    printf("  help         - Show this help message\n");
    printf("  timer <sec>  - Countdown timer\n");
    printf("  exit         - Exit NovaOS\n");
    printf("  echo <text>  - Print text\n");
    printf("  calc <expr>  - Simple calculator (e.g., 3+5 or 2^3)\n");
    printf("  sysinfo      - Show system info\n");
    printf("  log          - Show command log\n");
}

// --------------------------
void run_timer(int seconds) {
    for (int i = seconds; i > 0; i--) {
        printf("%d\n", i);
        fflush(stdout);
        Sleep(1000);
    }
    printf("Time's up!\n");
}

// --------------------------
void echo_text(const char *text) {
    if (text && text[0] != '\0') printf("%s\n", text);
    else printf("\n");
}

// --------------------------
void calc(const char *expr) {
    if (!expr) {
        printf("Usage: calc <a+b> or <a-b> or <a*b> or <a/b> or <a^b>\n");
        return;
    }

    double a, b;
    char op;
    // спочатку пробуємо як "a op b" без пробілів (наприклад 3+5)
    if (sscanf(expr, "%lf %c %lf", &a, &op, &b) == 3) {
        // успішно прочитано з пробілами
    } else if (sscanf(expr, "%lf%c%lf", &a, &op, &b) != 3) {
        printf("Invalid expression. Try like: calc 3+5 or calc 3 + 5\n");
        return;
    }

    switch (op) {
        case '+': printf("%.6g\n", a + b); break;
        case '-': printf("%.6g\n", a - b); break;
        case '*': printf("%.6g\n", a * b); break;
        case '/':
            if (b == 0.0) printf("Error: divide by zero\n");
            else printf("%.6g\n", a / b);
            break;
        case '^': {
            double r = pow(a, b);
            printf("%.6g\n", r);
            break;
        }
        default:
            printf("Unknown operator: %c\n", op);
    }
}

// --------------------------
void sysinfo() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    printf("System time: %02d:%02d:%02d\n", t.wHour, t.wMinute, t.wSecond);
    printf("Date: %02d/%02d/%04d\n", t.wDay, t.wMonth, t.wYear);
    printf("OS: Windows (NovaOS shell)\n");
}

// --------------------------
// додаємо в лог рядок з часовою міткою
void savecmd(const char *text) {
    if (!text) return;
    FILE *f = fopen("log.txt", "a");
    if (!f) return;

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    if (tm) {
        fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec,
            text);
    } else {
        fprintf(f, "%s\n", text);
    }
    fclose(f);
}

// --------------------------
// читаємо і виводимо лог
void readlog() {
    FILE *f = fopen("log.txt", "r");
    if (!f) {
        printf("(no log file yet)\n");
        return;
    }
    char line[512];
    printf("---- NovaOS log ----\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    printf("--------------------\n");
    fclose(f);
}
