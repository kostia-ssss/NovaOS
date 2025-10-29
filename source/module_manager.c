#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "module_manager.h"

#define MAX_COMMANDS 128
#define MAX_MODULES 32

typedef struct {
    char name[64];
    HMODULE handle;
} module_entry_t;

static command_t command_list[MAX_COMMANDS];
static module_entry_t modules[MAX_MODULES];
static int command_count = 0;
static int module_count = 0;

command_t *commands = command_list;

void register_command(const char *name, int (*func)(int,char**), const char *help, const char *module_name) {
    if (command_count >= MAX_COMMANDS) {
        printf("Command limit reached!\n");
        return;
    }
    command_list[command_count].name = _strdup(name);
    command_list[command_count].func = func;
    command_list[command_count].help = _strdup(help);
    command_list[command_count].module_name = module_name ? _strdup(module_name) : NULL;
    command_count++;
}

void unregister_command(const char *name) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(command_list[i].name, name) == 0) {
            free((void*)command_list[i].name);
            free((void*)command_list[i].help);
            free((void*)command_list[i].module_name);
            for (int j = i; j < command_count - 1; j++)
                command_list[j] = command_list[j + 1];
            command_count--;
            return;
        }
    }
}

command_t *find_command(const char *name) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(command_list[i].name, name) == 0)
            return &command_list[i];
    }
    return NULL;
}

// === Динамічне завантаження ===

typedef void (*mod_init_fn)(void (*register_fn)(const char*, int(*)(int,char**), const char*, const char*), const char*);
typedef void (*mod_deinit_fn)(void (*unregister_fn)(const char*));

int load_module(const char *fname) {
    if (module_count >= MAX_MODULES) {
        printf("Module limit reached!\n");
        return 1;
    }

    HMODULE hmod = LoadLibrary(fname);
    if (!hmod) {
        printf("Failed to load module: %s\n", fname);
        return 1;
    }

    mod_init_fn init = (mod_init_fn)GetProcAddress(hmod, "module_init");
    if (!init) {
        printf("module_init not found in %s\n", fname);
        FreeLibrary(hmod);
        return 1;
    }

    init(register_command, fname); // <--- передаємо ім'я DLL як module_name

    strncpy(modules[module_count].name, fname, sizeof(modules[module_count].name)-1);
    modules[module_count].handle = hmod;
    module_count++;

    printf("Module loaded: %s\n", fname);
    return 0;
}

int unload_module(const char *fname) {
    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, fname) == 0) {
            HMODULE hmod = modules[i].handle;
            mod_deinit_fn deinit = (mod_deinit_fn)GetProcAddress(hmod, "module_deinit");
            if (deinit) {
                deinit(unregister_command);
            }

            // Видаляємо всі команди, що належать цьому модулю
            for (int j = 0; j < command_count; j++) {
                if (command_list[j].module_name && strcmp(command_list[j].module_name, fname) == 0) {
                    unregister_command(command_list[j].name);
                    j--;
                }
            }

            FreeLibrary(hmod);

            for (int j = i; j < module_count - 1; j++)
                modules[j] = modules[j + 1];
            module_count--;

            printf("Module unloaded: %s\n", fname);
            return 0;
        }
    }

    printf("Module not found: %s\n", fname);
    return 1;
}
