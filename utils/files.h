#ifndef CHESS_FILES_H
#define CHESS_FILES_H

#include "../abstract/defs.h"

void createDirectory(char * path);
void createDirectoryIfMissing(char * path);

char * combinePath(char * a, char * b);

bool_t isPathFile(char * path);
char ** listDirectoryFiles(char * path, size_t * count);

#endif //CHESS_FILES_H
