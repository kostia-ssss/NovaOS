#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // atoi / strtol
#include <windows.h>  // Sleep, Beep

#define INPUT_SIZE 256

void change_color(char *c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (strcmp(c, "red") == 0) {
        SetConsoleTextAttribute(hConsole, 12); // червоний
    } else if (strcmp(c, "green") == 0) {
        SetConsoleTextAttribute(hConsole, 10); // зелений
    } else if (strcmp(c, "blue") == 0) {
        SetConsoleTextAttribute(hConsole, 9);  // синій
    } else if (strcmp(c, "yellow") == 0) {
        SetConsoleTextAttribute(hConsole, 14); // жовтий
    } else if (strcmp(c, "white") == 0) {
        SetConsoleTextAttribute(hConsole, 7);  // білий
    } else {
        printf("Unknown color: %s\n", c);
    }
}

void doc(char *func) {
    if (strcmp(func, "help") == 0) {
        printf("This command shows ALL commands!\n");
    } else if (strcmp(func, "exit") == 0) {
        printf("This command exits this OS :(\n");
    } else if (strcmp(func, "timer") == 0) {
        printf("This command starts timer on X seconds (e.g.: 'timer 5' - it starts timer on 5 seconds)!\n");
    } else if (strcmp(func, "clear") == 0) {
        printf("This command clears ALL text in console!\n");
    } else if (strcmp(func, "cc") == 0) {
        printf("This command changes color text. Colors: red, yellow, green, blue, white\n");
    }
}

void help() {
    printf("Commands:\n - help\n - exit\n - timer <seconds>\n - clear\n - cc <color>\n - doc <cmd>\n");
}

void clear() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD cellCount;
    DWORD count;
    COORD homeCoords = { 0, 0 };

    if (hConsole == INVALID_HANDLE_VALUE) return;

    /* Отримуємо інформацію про розмір буфера */
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Заповнюємо буфер пробілами */
    FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);

    /* Переміщаємо курсор на початок */
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void timer(int seconds) {
    if (seconds <= 0) {
        printf("Timer: некоректний час (%d)\n", seconds);
        return;
    }

    for (int i = seconds; i > 0; i--) {
        // Показуємо лічильник на одному рядку
        printf("\r%d second(s) remaining...  ", i);
        fflush(stdout);
        Sleep(1000);
    }

    printf("\rTime's up!                    \n");
    for (int i = 3; i > 0; i--) {
        Beep(1000, 300);
        Sleep(150);
    }
}

int main() {
    printf("Welcome to NovaOS!\n");
    char input[INPUT_SIZE];

    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) {
            // Помилка або EOF
            break;
        }

        // Прибираємо '\n'
        input[strcspn(input, "\n")] = '\0';

        // Якщо рядок порожній — пропускаємо
        if (input[0] == '\0') continue;

        // Розбиваємо на токени (команда і аргументи)
        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        // якщо потрібно — можна отримати arg2 = strtok(NULL, " ");

        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) {
            help();
        } else if (strcmp(cmd, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(cmd, "timer") == 0) {
            if (!arg1) {
                printf("Помилка: вкажи кількість секунд. Приклад: timer 5\n");
                continue;
            }

            // Більш безпечне перетворення — strtol
            char *endptr;
            long val = strtol(arg1, &endptr, 10);
            if (*endptr != '\0' || val <= 0) {
                printf("Помилка: некоректний аргумент '%s'\n", arg1);
                continue;
            }

            timer((int)val);
        } else if (strcmp(cmd, "clear") == 0) {
            clear();
        } else if (strcmp(cmd, "cc") == 0) {
            change_color(arg1);
        } else if (strcmp(cmd, "doc") == 0) {
            doc(arg1);
        } else {
            printf("Unknown command: %s\n", cmd);
        }
    }

    return 0;
}
