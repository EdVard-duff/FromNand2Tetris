#include "parser.h"
#include "utility.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <libgen.h>

static const char *typeLookup[] = {
    "C_ARITHMETIC",
    "C_PUSH",
    "C_POP",
    "C_LABEL",
    "C_GOTO",
    "C_IF",
    "C_FUNCTION",
    "C_RETURN",
    "C_CALL",
    "INVALID_COMMAND"
};

static void printCommand(Parser *parser) {
    printf("Command: %s\n", parser->curCommand);
    printf("Type: %s\n", typeLookup[parser->type]);
    printf("Arg1: %s\n", parser->arg1);
    printf("Arg2: %d\n", parser->arg2);
    printf("\n");
}

int parserInit(Parser *parser, const char *filename) {
    parser->filestream = fopen(filename, "r");
    if (parser->filestream == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return -1;
    }

    char filenameCopy[MAX_PATH_LENGTH];
    strcpy(filenameCopy, filename);

    char *dot = strrchr(filenameCopy, '.');
    if (dot)
    {
        *dot = '\0'; // Remove the extension
    }
    strcpy(parser->filename, basename(filenameCopy));    

    parser->curCommand[0] = '\0';
    parser->type = INVALID_COMMAND;
    parser->arg1[0] = '\0';

    return 0;
}

void parserFree(Parser *parser) {
    if (parser->filestream != NULL) {
        fclose(parser->filestream);
    }
}

int advance(Parser *parser) {
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, MAX_LINE_LENGTH, parser->filestream) == NULL) {
        return -1; 
    }

    char *trimmed = trimWhitespace(removeComments(buffer));
    if (strlen(trimmed) == 0) {
        return advance(parser);
    }
    strcpy(parser->curCommand, trimmed);
    setCommand(parser, trimmed);

    #ifdef TEST_PARSER
        printCommand(parser);
    #endif

    return 0;
}

void setCommand(Parser *parser, char *commandStr) {
    char *token = strtok(commandStr, " ");

    if (strcmp(token, "add") == 0 || strcmp(token, "sub") == 0 || strcmp(token, "neg") == 0 || strcmp(token, "eq") == 0 || strcmp(token, "gt") == 0 || strcmp(token, "lt") == 0 || strcmp(token, "and") == 0 || strcmp(token, "or") == 0 || strcmp(token, "not") == 0) {
        parser->type = C_ARITHMETIC;
        strcpy(parser->arg1, token);
        parser->arg2 = -1;
    } else if (strcmp(token, "push") == 0) {
        parser->type = C_PUSH;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        token = strtok(NULL, " ");
        parser->arg2 = atoi(token);
    } else if (strcmp(token, "pop") == 0) {
        parser->type = C_POP;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        token = strtok(NULL, " ");
        parser->arg2 = atoi(token);
    } else if (strcmp(token, "label") == 0) {
        parser->type = C_LABEL;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        parser->arg2 = -1;
    } else if (strcmp(token, "goto") == 0) {
        parser->type = C_GOTO;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        parser->arg2 = -1;
    } else if (strcmp(token, "if-goto") == 0) {
        parser->type = C_IF;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        parser->arg2 = -1;
    } else if (strcmp(token, "function") == 0) {
        parser->type = C_FUNCTION;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        token = strtok(NULL, " ");
        parser->arg2 = atoi(token);
    } else if (strcmp(token, "return") == 0) {
        parser->type = C_RETURN;
        parser->arg1[0] = '\0';
        parser->arg2 = -1;
    } else if (strcmp(token, "call") == 0) {
        parser->type = C_CALL;
        token = strtok(NULL, " ");
        strcpy(parser->arg1, token);
        token = strtok(NULL, " ");
        parser->arg2 = atoi(token);
    } else {
        parser->type = INVALID_COMMAND;
        parser->arg1[0] = '\0';
        parser->arg2 = -1;
    }
}

const char *getArg1(Parser *parser) {
    if (parser->type == C_RETURN || parser->type == INVALID_COMMAND) {
        return "";
    }
    return parser->arg1;
}

int getArg2(Parser *parser) {
    CommandType type = parser->type;
    if (type == C_PUSH || type == C_POP || type == C_FUNCTION || type == C_CALL) {
        return parser->arg2;
    }
    return -1; 
}

