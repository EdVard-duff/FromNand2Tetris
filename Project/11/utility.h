#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

// 定义最大路径长度
#define MAX_PATH_LENGTH 1024

// 根据操作系统定义路径分隔符
#ifdef _WIN32
    #define PATH_SEPARATOR '\\'
#else
    #define PATH_SEPARATOR '/'
#endif

/**
 * @brief Trims leading and trailing whitespace characters from a string.
 *
 * This function takes a string as input and removes any leading and trailing whitespace characters from it.
 * The modified string is then returned.
 *
 * @param str The string to be trimmed.
 * @return The trimmed string.
 */
char *trimWhitespace(char *str);


/**
 * Reads a valid line from the specified file and stores it in the destination buffer. Comments of C style and leading/trailing whitespace are removed.
 *
 * @param fp The file pointer to read from.
 * @param dest The destination buffer to store the line. 
 * @param n The maximum number of characters to read.
 * 
 * @return If reaches the end of file or there is no valid line, return -1, dest will be an empty string. Otherwise, return 0.
 */
int read_valid_line(FILE *fp, char *dest, int n); 

/**
 * @brief Checks if a given filename has a specific suffix.
 *
 * This function takes a filename and a suffix as input parameters and checks if the filename ends with the specified suffix.
 *
 * @param filename The filename to check.
 * @param suffix The suffix to compare against the filename.
 * @return Returns 1 if the filename has the specified suffix, otherwise returns 0.
 */
int has_suffix(const char *filename, const char *suffix); 

void generateOutputFilepath(char *dest, const char *input_path, const char *outDir, const char *outSuffix);
#endif // UTILITY_H