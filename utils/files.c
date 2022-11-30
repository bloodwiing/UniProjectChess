#include "files.h"
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __linux__
void createDirectory(char * name) {
    mkdir(name, 0600);
}
#elif defined(_WIN32)
#include<direct.h>
void createDirectory(char * name) {
    _mkdir(name);
}
#endif

void createDirectoryIfMissing(char * name) {
    struct stat st = {0};

    if (stat("/some/directory", &st) == -1) {
        createDirectory(name);
    }
}
