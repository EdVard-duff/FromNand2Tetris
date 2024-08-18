#include "utility.h"
#include <ctype.h>
#include <string.h>

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