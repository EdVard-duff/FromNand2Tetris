#include "parser.h"
#include "utility.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

void printCommand(Parser *parser) {
    Command command = parser->currentCommand;
    CommandType type = command.type;
    switch (type) {
        case C_ARITHMETIC:
            printf("Arithmetic command: %d\n", command.arithmetic);
            break;
        case C_PUSH:
            printf("Push command: %s %d\n", getArg1(parser), command.offset);
            break;
        case C_POP:
            printf("Pop command: %s %d\n", getArg1(parser), command.offset);
            break;
        default:
            fprintf(stderr, "Error: Invalid command type\n");
            exit(1);
    }
}

void parserInit(Parser *parser, const char *filename) {
    parser->filestream = fopen(filename, "r");
    if (parser->filestream == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }
}

int advance(Parser *parser) {
    char buffer[MAX_COMMAND_LENGTH];
    if (fgets(buffer, MAX_COMMAND_LENGTH, parser->filestream) == NULL) {
        return -1; 
    }

    char *trimmed = trimWhitespace(removeComments(buffer));
    if (strlen(trimmed) == 0) {
        return advance(parser);
    }
    setCommand(parser, trimmed);
    printCommand(parser);
    return 0;
}

const char *getCommandType(Parser *parser) {
    CommandType type = parser->currentCommand.type;
    switch (type) {
        case C_ARITHMETIC:
            return "C_ARITHMETIC";
        case C_PUSH:
            return "C_PUSH";
        case C_POP:
            return "C_POP";
        case C_LABEL:
            return "C_LABEL";
        case C_GOTO:
            return "C_GOTO";
        case C_IF:
            return "C_IF";
        case C_FUNCTION:
            return "C_FUNCTION";
        case C_RETURN:
            return "C_RETURN";
        case C_CALL:
            return "C_CALL";
        default:
            return "INVALID_COMMAND";
    }
}

const char *getArg1(Parser *parser) {
    SegmentType segment = parser->currentCommand.segment;
    switch (segment) {
        case SEG_LOCAL:
            return "local";
        case SEG_ARGUMENT:
            return "argument";
        case SEG_THIS:
            return "this";
        case SEG_THAT:
            return "that";
        case SEG_CONSTANT:
            return "constant";
        case SEG_STATIC:
            return "static";
        case SEG_TEMP:
            return "temp";
        case SEG_POINTER:
            return "pointer";
        default:
            return "INVALID_SEGMENT";
    }   
}

int getArg2(Parser *parser) {
    return parser->currentCommand.offset;
}

void setCommand(Parser *parser, char *command) {
    char *token = strtok(command, " ");

    if (strcmp(token, "push") == 0) {
        parser->currentCommand.type = C_PUSH;
        parser->currentCommand.segment = parseSegmentType(strtok(NULL, " "));
        parser->currentCommand.offset = atoi(strtok(NULL, " "));
        parser->currentCommand.arithmetic = INVALID_ARITHMETIC;
    } else if (strcmp(token, "pop") == 0) {
        parser->currentCommand.type = C_POP;
        parser->currentCommand.segment = parseSegmentType(strtok(NULL, " "));
        parser->currentCommand.offset = atoi(strtok(NULL, " "));
        parser->currentCommand.arithmetic = INVALID_ARITHMETIC;
    } else {
        parser->currentCommand.type = C_ARITHMETIC;
        parser->currentCommand.segment = INVALID_SEGMENT;
        parser->currentCommand.offset = -1;
        parser->currentCommand.arithmetic = parseArithmeticType(token);
    }
}

SegmentType parseSegmentType(const char *segment) {
    if (strcmp(segment, "local") == 0) {
        return SEG_LOCAL;
    } else if (strcmp(segment, "argument") == 0) {
        return SEG_ARGUMENT;
    } else if (strcmp(segment, "this") == 0) {
        return SEG_THIS;
    } else if (strcmp(segment, "that") == 0) {
        return SEG_THAT;
    } else if (strcmp(segment, "constant") == 0) {
        return SEG_CONSTANT;
    } else if (strcmp(segment, "static") == 0) {
        return SEG_STATIC;
    } else if (strcmp(segment, "temp") == 0) {
        return SEG_TEMP;
    } else if (strcmp(segment, "pointer") == 0) {
        return SEG_POINTER;
    } else {
        fprintf(stderr, "Error: Invalid segment type\n");
        exit(1);
    }
}

ArithmeticType parseArithmeticType(const char *command) {
    if (strcmp(command, "add") == 0) {
        return ADD;
    } else if (strcmp(command, "sub") == 0) {
        return SUB;
    } else if (strcmp(command, "neg") == 0) {
        return NEG;
    } else if (strcmp(command, "eq") == 0) {
        return EQ;
    } else if (strcmp(command, "gt") == 0) {
        return GT;
    } else if (strcmp(command, "lt") == 0) {
        return LT;
    } else if (strcmp(command, "and") == 0) {
        return AND;
    } else if (strcmp(command, "or") == 0) {
        return OR;
    } else if (strcmp(command, "not") == 0) {
        return NOT;
    } else {
        fprintf(stderr, "Error: Invalid arithmetic type\n");
        exit(1);
    }
}