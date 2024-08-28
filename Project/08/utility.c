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