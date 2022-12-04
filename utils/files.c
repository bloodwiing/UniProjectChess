#include "files.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __linux__
void createDirectory(char * name) {
    mkdir(name, 0775);
}
#elif defined(_WIN32)
#include <direct.h>
#include <stdio.h>

void createDirectory(char * name) {
    _mkdir(name);
}
#endif

void createDirectoryIfMissing(char * name) {
    struct stat st = {0};

    if (stat(name, &st) == -1) {
        createDirectory(name);
    }
}

char * combinePath(char * a, char * b) {
    char * full = malloc(strlen(a) + strlen(b) + 2);
    sprintf(full, "%s/%s", a, b);
    return full;
}

bool_t isPathFile(char * path) {
    struct stat path_stat;
    stat(path, &path_stat);
    if (path == 0) return false;
    return S_ISREG(path_stat.st_mode) != false;
}

char ** listDirectoryFiles(char * path, size_t * count) {
    struct dirent *de;
    DIR *dr = opendir(path);
    if (dr == NULL) {
        *count = 0;
        return NULL;
    }

    size_t total_count = 0;
    while (readdir(dr) != NULL) { ++total_count; }
    char ** files = malloc(sizeof(char *) * total_count);
    seekdir(dr, 0);

    while ((de = readdir(dr)) != NULL) {
        char * full = combinePath(path, de->d_name);
        if (isPathFile(full)) {
            files[*count] = malloc(strlen(de->d_name) + 1);
            strcpy(files[(*count)++], de->d_name);
        }
        free(full);
    }

    files = realloc(files, sizeof(char *) * (*count));
    closedir(dr);

    return files;
}