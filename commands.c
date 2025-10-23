#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include "commands.h"

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
    printf("Commands:\n - help\n - exit\n - timer <seconds>\n - clear\n - cc <color>\n - doc <cmd>\n");
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

void timer(int seconds) {
    if (seconds <= 0) {
        printf("Timer: invalid value (%d)\n", seconds);
        return;
    }

    for (int i = seconds; i > 0; i--) {
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
