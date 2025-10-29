#include <direct.h>  // для _getcwd
#include <limits.h>  // для _MAX_PATH
#include <stdio.h>
#include <windows.h>

int rm(int argc, char *argv[]) {
    if (remove(argv[1]) == 0)
        printf("File '%s' deleted successfully.\n", argv[1]);
    else
        printf("Cannot delete this file\n");
    return 0;
}

int touch(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Cannot create this file\n");
        return 1;
    }
    fclose(file);
    printf("File '%s' created.\n", argv[1]);
    return 0;
}

int cat(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Cannot open this file\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
    return 0;
}

int mkdir_win(int argc, char *argv[]) {
    if (CreateDirectory(argv[1], NULL)) {
        printf("Folder '%s' created successfully.\n", argv[1]);
    } else {
        DWORD err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS) {
            printf("Folder arleady exists");
        } else {
            printf("Cannot create this folder\n");
        }
    }
    return 0;
}

int rmdir_win(int argc, char *argv[]) {
    if (RemoveDirectory(argv[1])) {
        printf("Folder '%s' deleted successfully.\n", argv[1]);
    }
    return 0;
}

int cd(int argc, char *argv[]) {
    if (_chdir(argv[1]) == 0) {
        printf("Directory changed to: %s\n", argv[1]);
    } else {
        printf("Invalid path\n");
    }
    return 0;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void module_init(void (*register_fn)(const char*, int(*)(int,char**), const char*, const char*), const char *modname) {
    register_fn("rm", rm, "Removes file: rm <file>", modname);
    register_fn("touch", touch, "Creates file: touch <file>", modname);
    register_fn("cat", cat, "Reads file: cat <file>", modname);
    register_fn("mkdir", mkdir_win, "Creates new folder: mkdir <folder name>", modname);
    register_fn("rmdir", rmdir_win, "Removes folder: rmdir <file>", modname);
    register_fn("cd", cd, "Changes directory to path: cd <path>", modname);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void module_deinit(void (*unregister_fn)(const char*)) {
    unregister_fn("rm");
    unregister_fn("touch");
    unregister_fn("cat");
    unregister_fn("mkdir");
    unregister_fn("rmdir");
    unregister_fn("cd");
}