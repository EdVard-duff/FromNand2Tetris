#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include "utility.h"

char *trimWhitespace(char *str)
{
    // Trim leading whitespace
    while (isspace(*str))
    {
        str++;
    }
    if (*str == '\0') // All spaces
    { 
        return str;
    }

    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
    {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

char *removeComments(char *str)
{
    char *comment = strstr(str, "//");
    if (comment != NULL)
    {
        *comment = '\0';
    }
    return str;
}

int has_suffix(const char *filename, const char *suffix) {
    size_t len_filename = strlen(filename);
    size_t len_suffix = strlen(suffix);

    if (len_filename < len_suffix) {
        return 0;
    }

    return strcmp(filename + len_filename - len_suffix, suffix) == 0;
}

void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
    
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int read_valid_line(FILE *fp, char *dest, int n) {
    char buffer[1024]; // 临时缓冲区用于存储文件中的一行
    char *p;
    static int in_multiline_comment = 0; // 静态变量，用于跨行跟踪多行注释
    int in_string = 0; // 追踪是否在字符串内部

    // 读取文件中的一行
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        p = buffer;

        // 如果处于多行注释中，先处理注释的结束
        if (in_multiline_comment) {
            char *end_comment = strstr(p, "*/");
            if (end_comment) {
                p = end_comment + 2; // 跳过 "*/"
                in_multiline_comment = 0;
            } else {
                // 整行仍在多行注释中，继续读取下一行
                continue;
            }
        }

        // 删除行首空白字符
        while (isspace((unsigned char)*p)) {
            p++;
        }

        // 如果当前行是空行或只有注释，跳过
        if (*p == '\0') {
            continue;
        }

        // 处理当前行中的字符
        char *output = dest;
        int chars_written = 0;
        while (*p != '\0' && chars_written < n - 1) {
            if (in_string) {
                // 在字符串内部，找到结束的引号前不处理注释
                if (*p == '"') {
                    in_string = 0;
                }
                *output++ = *p++;
                chars_written++;
            } else {
                // 检查是否进入字符串
                if (*p == '"') {
                    in_string = 1;
                    *output++ = *p++;
                    chars_written++;
                }
                // 检查是否遇到单行注释 "//"
                else if (*p == '/' && *(p + 1) == '/') {
                    break; // 跳过 "//" 之后的内容
                }
                // 检查是否遇到多行注释 "/*"
                else if (*p == '/' && *(p + 1) == '*') {
                    in_multiline_comment = 1;
                    p = strstr(p, "*/"); // 跳过整个多行注释
                    if (p) {
                        p += 2;
                        in_multiline_comment = 0;
                    } else {
                        break; // 多行注释未结束，跳过当前行
                    }
                }
                // 处理正常字符
                else {
                    *output++ = *p++;
                    chars_written++;
                }
            }
        }

        // 删除行尾空白字符
        while (output > dest && isspace((unsigned char)*(output - 1))) {
            output--;
        }

        *output = '\0'; // 添加 null 终止符

        // 如果去除空白和注释后还有有效内容，返回这一行
        if (output != dest) {
            return 0;
        }
    }

    // 文件结束或没有有效行，返回空字符串
    dest[0] = '\0';
    return -1;
}

void generateOutputFilepath(char *dest, const char *input_path, const char *outDir, const char *outSuffix)
{
    char filenameCopy[MAX_PATH_LENGTH];
    strncpy(filenameCopy, input_path, MAX_PATH_LENGTH);

    snprintf(dest, MAX_PATH_LENGTH, "%s%c%s", outDir, PATH_SEPARATOR, basename(filenameCopy));
    char *dot = strrchr(dest, '.');
    if (dot != NULL)
    {
        strncpy(dot + 1, outSuffix, MAX_PATH_LENGTH - (dot - dest));
    }
}