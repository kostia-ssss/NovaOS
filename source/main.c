#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "module_manager.h"

void get_path(char *out_buf, size_t buf_size) {
    if (!out_buf || buf_size == 0) return;

    DWORD length = GetCurrentDirectoryA((DWORD)buf_size, out_buf);
    if (length == 0 || length >= buf_size) {
        // Помилка або потрібно більше місця — зробимо fallback
        // Якщо помилка — ставимо "."
        strncpy(out_buf, ".", buf_size);
        out_buf[buf_size - 1] = '\0';
    }
}

int main() {
    char input[256];
    printf("Welcome to NovaOS Shell!\n");
    printf("Type 'help' or 'loadmod <file>' to begin.\n");

    while (1) {
        char cur_path[MAX_PATH];
        get_path(cur_path, sizeof(cur_path));
        printf("%s >>> ", cur_path);

        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        char *argv[10];
        int argc = 0;
        char *token = strtok(input, " ");
        while (token && argc < 10) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        if (argc == 0) continue;

        if (strcmp(argv[0], "exit") == 0) break;
        else if (strcmp(argv[0], "help") == 0) {
            printf("Available commands:\n");
            for (int i = 0; commands[i].name != NULL; i++)
                printf(" - %-12s : %s\n", commands[i].name, commands[i].help);
            printf("Built-in: help, exit, loadmod, unloadmod\n");
        }
        else if (strcmp(argv[0], "loadmod") == 0 && argc == 2) {
            load_module(argv[1]);
        }
        else if (strcmp(argv[0], "unloadmod") == 0 && argc == 2) {
            unload_module(argv[1]);
        }
        else {
            command_t *cmd = find_command(argv[0]);
            if (cmd) cmd->func(argc, argv);
            else printf("Unknown command: %s\n", argv[0]);
        }
    }

    printf("Goodbye!\n");
    return 0;
}
