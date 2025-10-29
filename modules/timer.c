#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static int timer_cmd(int argc, char *argv[]) {
    if (argc == 2) {
        int total = atoi(argv[1]);
        if (total <= 0) {
            printf("Invalid time.\n");
            return 1;
        }
        printf("Timer started for %d second(s)\n", total);
        for (int i = total; i > 0; i--) {
            printf("\rRemains: %d second(s)   ", i);
            fflush(stdout);
            Sleep(1000);
        }
        printf("\rTime's up!                          \n");
        for (int i = 0; i < 3; i++)
        {
            Beep(1000, 500);
        }
    }
    else if (argc == 3) {
        int m = atoi(argv[2]);
        int s = atoi(argv[1]);
        int total = m*60 + s;
        if (total <= 0) {
            printf("Invalid time.\n");
            return 1;
        }
        printf("Timer started for %d minute(s) %d second(s)\n", m, s);
        for (int i = total; i > 0; i--) {
            printf("\rRemains: %d minute(s) %d second(s)   ", i/60, i%60);
            fflush(stdout);
            Sleep(1000);
        }
        printf("\rTime's up!                          \n");
        for (int i = 0; i < 3; i++)
        {
            Beep(1000, 500);
        }
    }
    else if (argc == 4) {
        int h = atoi(argv[3]);
        int m = atoi(argv[2]);
        int s = atoi(argv[1]);
        int total = h*3600 + m*60 + s;
        if (total <= 0) {
            printf("Invalid time.\n");
            return 1;
        }
        printf("Timer started for %d hour(s) %d minute(s) %d second(s)\n", h, m, s);
        for (int i = total; i > 0; i--) {
            printf("\rRemains: %d hour(s) %d minute(s) %d second(s)   ", i/3600, i/60, i%60);
            fflush(stdout);
            Sleep(1000);
        }
        printf("\rTime's up!                          \n");
        for (int i = 0; i < 3; i++)
        {
            Beep(1000, 500);
        }
        
    }
    else {
        printf("Usage: timer <seconds>\n");
        printf("Usage: timer <seconds> <minutes>\n");
        printf("Usage: timer <seconds> <minutes> <hours>\n");
    }
    return 0;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void module_init(void (*register_fn)(const char*, int(*)(int,char**), const char*, const char*), const char *modname) {
    register_fn("timer", timer_cmd, "Run countdown timer", modname);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void module_deinit(void (*unregister_fn)(const char*)) {
    unregister_fn("timer");
}
