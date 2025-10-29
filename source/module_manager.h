#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

typedef struct {
    const char *name;
    int (*func)(int argc, char *argv[]);
    const char *help;
    const char *module_name;
} command_t;

// Реєстрація/видалення команд
void register_command(const char *name, int (*func)(int,char**), const char *help, const char *module_name);
void unregister_command(const char *name);

// Завантаження / розвантаження модулів
int load_module(const char *fname);
int unload_module(const char *fname);

// Пошук команди
command_t *find_command(const char *name);

// Список команд
extern command_t *commands;

#endif
