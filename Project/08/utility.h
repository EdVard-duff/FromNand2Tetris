#ifndef UTILITY_H
#define UTILITY_H

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
 * Removes comments from a given string.
 *
 * @param str The string from which comments will be removed.
 * @return A pointer to the modified string without comments.
 */
char *removeComments(char *str);

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
#endif // UTILITY_H