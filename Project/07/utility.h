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
 * Generates the output path based on the input path, output directory, and suffix.
 *
 * @param input_path The path of the input file.
 * @param output_dir The directory where the output file will be saved.
 * @param suffix The suffix to be added to the output file name.
 * @param output_path The buffer to store the generated output path.
 */
void generate_output_path(const char* input_path, const char* output_dir, const char* suffix, char* output_path);
#endif // UTILITY_H