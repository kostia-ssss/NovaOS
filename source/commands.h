#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct {
    const char *name;
    int (*func)(int argc, char *argv[]);
    const char *help;
    const char *module_name; // опційно: зв'язок з модулем
} command_t;

// Оголошення — інші файли бачать, що масив існує
extern command_t commands[];

int help(int argc, char *argv[]);
int echo(int argc, char *argv[]);
int timer(int argc, char *argv[]);

#endif // COMMANDS_H
