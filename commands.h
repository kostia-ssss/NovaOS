#ifndef COMMANDS_H
#define COMMANDS_H

void help();
void timer(int seconds, int minutes, int hours);
void clear();
void change_color(char *c);
void doc(char *func);
void theme(char *bgColor, char *textColor);
void echo(char *text);
void music();
void time_();
void touch(char *filename);
void cat(char *filename);
void rm(char *filename);

#endif
