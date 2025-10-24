#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "commands.h"
#include "utils.h"

void change_color(char *c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!c) return;

    if (strcmp(c, "red") == 0) SetConsoleTextAttribute(hConsole, 12);
    else if (strcmp(c, "green") == 0) SetConsoleTextAttribute(hConsole, 10);
    else if (strcmp(c, "blue") == 0) SetConsoleTextAttribute(hConsole, 9);
    else if (strcmp(c, "yellow") == 0) SetConsoleTextAttribute(hConsole, 14);
    else if (strcmp(c, "white") == 0) SetConsoleTextAttribute(hConsole, 7);
    else printf("Unknown color: %s\n", c);
}

void doc(char *func) {
    if (!func) return;

    if (strcmp(func, "help") == 0)
        printf("This command shows ALL commands!\n");
    else if (strcmp(func, "exit") == 0)
        printf("This command exits this OS :(\n");
    else if (strcmp(func, "timer") == 0)
        printf("Starts timer on X seconds (e.g.: 'timer 5')\n");
    else if (strcmp(func, "clear") == 0)
        printf("Clears all text in console\n");
    else if (strcmp(func, "cc") == 0)
        printf("Changes text color. Colors: red, yellow, green, blue, white\n");
    else
        printf("No documentation for '%s'\n", func);
}

void help() {
    printf("Commands:\n - help\n - exit\n - timer <seconds> <minutes> <hours>\n - clear\n - cc <color>\n - doc <cmd>\n - theme <bg color> <text color>\n - time\n - music\n - echo <text>\n");
}

void clear() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD cellCount, count;
    COORD homeCoords = {0,0};

    if (hConsole == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void timer(int seconds, int minutes, int hours) {
    int total = seconds + minutes * 60 + hours * 3600;

    if (total <= 0) {
        printf("Timer: invalid value (%d)\n", total);
        return;
    }

    for (int i = total; i > 0; i--) {
        int h = i / 3600;
        int m = (i % 3600) / 60;
        int s = i % 60;

        printf("\r%02d hour(s) %02d minute(s) %02d second(s) remaining...", h, m, s);
        fflush(stdout);
        Sleep(1000);
    }

    printf("\rTime's up!                                                                 \n");
    for (int i = 0; i < 3; i++) {
        Beep(1000, 300);
        Sleep(150);
    }
}


void theme(char *bgColor, char *textColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int bg = get_color_code(bgColor);
    int text = get_color_code(textColor);

    int color = (bg << 4) | text;
    SetConsoleTextAttribute(hConsole, color);

    printf("Theme updated! Background: %s, Text: %s\n",
           bgColor ? bgColor : "default",
           textColor ? textColor : "default");

    Config cfg = load_config(); // завантажуємо конфіг при старті

    // десь при зміні теми
    strcpy(cfg.theme_bg, bgColor);
    strcpy(cfg.theme_text, textColor);
    save_config(cfg); // зберігаємо зміни
}

void echo(char *text) {
    printf("%s\n", text);
}

void music() {
    int temp = 200;
    for (size_t i = 0; i < 8; i++)
    {
        Beep(208, temp);
        Beep(277, temp);
        Beep(330, temp);
    }
    for (size_t i = 0; i < 2; i++)
    {
        Beep(220, temp);
        Beep(277, temp);
        Beep(330, temp);
    }
    for (size_t i = 0; i < 2; i++)
    {
        Beep(220, temp);
        Beep(294, temp);
        Beep(370, temp);
    }
    Beep(208, temp);
    Beep(262, temp);
    Beep(370, temp);
    Beep(208, temp);
    Beep(277, temp);
    Beep(330, temp);
    Beep(208, temp);
    Beep(277, temp);
    Beep(311, temp);
    Beep(185, temp);
    Beep(262, temp);
    Beep(311, temp);
}

void time_() {
    time_t now;           // змінна для часу
    struct tm *local;     // структура для збереження локального часу

    time(&now);           // отримуємо поточний час (у секундах з 1970)
    local = localtime(&now);  // конвертуємо у "людську" форму

    printf("Current time: %02d:%02d:%02d\n",
           local->tm_hour,
           local->tm_min,
           local->tm_sec);
}

void rm(char *filename) {
    if (remove(filename) == 0)
        printf("File '%s' deleted successfully.\n", filename);
    else
        printf("Error: cannot delete file '%s'.\n", filename);
}

void touch(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: cannot create file '%s'.\n", filename);
        return;
    }
    fclose(file);
    printf("File '%s' created.\n", filename);
}

void cat(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: cannot open file '%s'.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}