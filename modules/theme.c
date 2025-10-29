#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* Trim helpers */
static void trim_inplace(char *s) {
    if (!s) return;
    // left
    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);

    // right
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) {
        s[len-1] = '\0';
        len--;
    }
}

/* Normalize: lowercase and replace '-' with nothing */
static void normalize_name(char *dst, const char *src, size_t dst_size) {
    if (!dst || !src || dst_size == 0) return;
    size_t i = 0;
    for (; *src && i + 1 < dst_size; ++src) {
        char c = *src;
        if (c == '-' || c == '_') continue;
        dst[i++] = (char)tolower((unsigned char)c);
    }
    dst[i] = '\0';
}

/* Приводимо назву кольору до коду 0..15 (Windows console color codes) */
/* Стандарт: 0=black,1=blue,2=green,3=cyan,4=red,5=magenta,6=yellow/brown,7=lightgray,
   8=darkgray,9=lightblue,10=lightgreen,11=lightcyan,12=lightred,13=lightmagenta,
   14=lightyellow,15=white */
int get_color_code(const char *color) {
    if (!color) return 7; // default = light gray

    char tmp[64];
    normalize_name(tmp, color, sizeof(tmp));
    trim_inplace(tmp);

    if (tmp[0] == '\0') return 7;

    /* synonyms and full names */
    if (strcmp(tmp, "black") == 0) return 0;
    if (strcmp(tmp, "blue") == 0) return 1;
    if (strcmp(tmp, "green") == 0) return 2;
    if (strcmp(tmp, "cyan") == 0 || strcmp(tmp, "aqua") == 0) return 3;
    if (strcmp(tmp, "red") == 0) return 4;
    if (strcmp(tmp, "magenta") == 0 || strcmp(tmp, "purple") == 0) return 5;
    if (strcmp(tmp, "yellow") == 0 || strcmp(tmp, "brown") == 0) return 6;
    if (strcmp(tmp, "lightgray") == 0 || strcmp(tmp, "grey") == 0 || strcmp(tmp, "gray") == 0) return 7;
    if (strcmp(tmp, "darkgray") == 0 || strcmp(tmp, "darkgrey") == 0) return 8;
    if (strcmp(tmp, "lightblue") == 0) return 9;
    if (strcmp(tmp, "lightgreen") == 0) return 10;
    if (strcmp(tmp, "lightcyan") == 0) return 11;
    if (strcmp(tmp, "lightred") == 0 || strcmp(tmp, "lightred") == 0) return 12;
    if (strcmp(tmp, "lightmagenta") == 0) return 13;
    if (strcmp(tmp, "lightyellow") == 0 || strcmp(tmp, "lightbrown") == 0) return 14;
    if (strcmp(tmp, "white") == 0 || strcmp(tmp, "brightwhite") == 0) return 15;

    /* support "light" prefix like "lightred" or "light-red" */
    if (strncmp(tmp, "light", 5) == 0) {
        char base[64];
        normalize_name(base, tmp + 5, sizeof(base));
        if (base[0] != '\0') {
            // map base colors to bright variants
            if (strcmp(base, "blue") == 0) return 9;
            if (strcmp(base, "green") == 0) return 10;
            if (strcmp(base, "cyan") == 0 || strcmp(base, "aqua") == 0) return 11;
            if (strcmp(base, "red") == 0) return 12;
            if (strcmp(base, "magenta") == 0 || strcmp(base, "purple") == 0) return 13;
            if (strcmp(base, "yellow") == 0 || strcmp(base, "brown") == 0) return 14;
            if (strcmp(base, "gray") == 0 || strcmp(base, "grey") == 0) return 7; // lightgray
        }
    }

    /* try numeric string "0".."15" */
    int num = atoi(tmp);
    if (num >= 0 && num <= 15) return num;

    /* fallback */
    return 7;
}

/* Твоя theme: тепер без static (щоб було очевидно експортування). */
/* Очікує: argc == 3, argv[1] = bg, argv[2] = text */
int theme(int argc, char *argv[]) {
    if (argc == 3) {
        char *bgColor = argv[1];
        char *textColor = argv[2];

        /* Trim possible newline / spaces */
        char bgbuf[64], txtbuf[64];
        if (bgColor) {
            strncpy(bgbuf, bgColor, sizeof(bgbuf)-1);
            bgbuf[sizeof(bgbuf)-1] = '\0';
            trim_inplace(bgbuf);
        } else bgbuf[0] = '\0';

        if (textColor) {
            strncpy(txtbuf, textColor, sizeof(txtbuf)-1);
            txtbuf[sizeof(txtbuf)-1] = '\0';
            trim_inplace(txtbuf);
        } else txtbuf[0] = '\0';

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) return 1;

        int bg = get_color_code(bgbuf);
        int text = get_color_code(txtbuf);

        int color = (bg << 4) | (text & 0x0F);
        SetConsoleTextAttribute(hConsole, (WORD)color);
    }
    else {
        printf("Usage: theme <bg color> <text color>\n");
        return 1;
    }

    return 0;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void module_init(void (*register_fn)(const char*, int(*)(int,char**), const char*, const char*), const char *modname) {
    /* register_fn(name, function, help, module_name) */
    register_fn("theme", theme, "Changes color of text and bg", modname);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void module_deinit(void (*unregister_fn)(const char*)) {
    unregister_fn("theme");
}
