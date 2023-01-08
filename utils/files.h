#ifndef CHESS_FILES_H
#define CHESS_FILES_H

#include "abstract/defs.h"

/// Creates a directory at the given path
/// \param path The path of the new directory
void createDirectory(char * path);

/// A conditional function to create a directory. If the directory already exists, this does nothing.
/// \param path The path of the new directory
void createDirectoryIfMissing(char * path);


/// Combines 2 paths together to a single path string
/// \param a The parent side of the path
/// \param b The child side of the path
/// \return A pointer to a new string (make sure to free it!)
char * combinePath(char * a, char * b);


/// Checks if the entry at the given path is a file
/// \param path The path of the file/folder
/// \return true if the path leads to a file, false otherwise
bool_t isPathFile(char * path);

/// Lists all files within a directory of the given path
/// \param path The path to the directory
/// \param count A pointer to a number that saves and tracks how many files have been found
/// \return A dynamic array of dynamic strings, each one being a file name (make sure to free each string and the whole array!)
char ** listDirectoryFiles(char * path, size_t * count);

#endif //CHESS_FILES_H
