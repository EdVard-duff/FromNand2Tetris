#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *trimWhitespace(char *str)
{
	// Trim leading whitespace
	while (isspace(*str))
	{
		str++;
	}
	if (*str == '\0')
	{ // All spaces
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

Parser *parserInit(const char *filename)
{
	Parser *parser = (Parser *)malloc(sizeof(Parser));
	if (parser == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for parser\n");
		exit(EXIT_FAILURE);
	}

	int errFlag = 0; // 错误标志, 用来简化控制流程, 否则容易忘记释放 parser.
	parser->file = fopen(filename, "r");
	if (parser->file == NULL)
	{
		fprintf(stderr, "Failed to open file %s\n", filename);
		errFlag = 1;
	}
	strcpy(parser->filename, filename);
	parser->currentCommand[0] = '\0';
	parser->commandType = INVALID_COMMAND;
	parser->heapAddress = HEAP_START;
	parser->lineNumber = -1;
	parser->address = -1;
	parser->symbol[0] = '\0';
	parser->dest[0] = '\0';
	parser->comp[0] = '\0';
	parser->jump[0] = '\0';

	if (errFlag)
	{
		parserFree(parser);
		exit(EXIT_FAILURE);
	}
	return parser;
}

void parserFree(Parser *parser)
{
	if (parser->file != NULL)
	{
		fclose(parser->file);
	}
	free(parser);
}

void resetParser(Parser *parser)
{
	rewind(parser->file);
	parser->currentCommand[0] = '\0';
	parser->commandType = INVALID_COMMAND;
	parser->heapAddress = HEAP_START;
	parser->lineNumber = -1;
	parser->address = -1;
	parser->symbol[0] = '\0';
	parser->dest[0] = '\0';
	parser->comp[0] = '\0';
	parser->jump[0] = '\0';
}

int hasMoreCommands(Parser *parser)
{
	return !feof(parser->file);
}

int advance(Parser *parser)
{
	char buffer[MAX_COMMAND_LENGTH];
	if (fgets(buffer, sizeof(buffer), parser->file) == NULL)
	{
		// printf("No more commands\n");
		return -1;
	}

	// Remove comments
	char *comment = strchr(buffer, '/');
	if (comment && comment[1] == '/')
	{
		*comment = '\0';
	}

	// Trim whitespace
	char *trimmed = trimWhitespace(buffer);
	if (strlen(trimmed) == 0)
	{
		// Skip empty lines
		return advance(parser);
	}

	// Copy trimmed command to currentCommand
	strncpy(parser->currentCommand, trimmed, sizeof(parser->currentCommand) - 1);
	parser->currentCommand[sizeof(parser->currentCommand) - 1] = '\0';
	setCommandType(parser);

	return 0;
}

void setCommandType(Parser *parser)
{
	if (parser->currentCommand[0] == '@')
	{
		parser->commandType = A_COMMAND;
		parser->lineNumber++;
		if (isdigit(parser->currentCommand[1]))
		{
			parser->address = atoi(parser->currentCommand + 1);
		}
		else
		{
			strncpy(parser->symbol, parser->currentCommand + 1, sizeof(parser->symbol) - 1);
			parser->symbol[sizeof(parser->symbol) - 1] = '\0';
		}
	}
	else if (parser->currentCommand[0] == '(')
	{
		parser->commandType = L_COMMAND;
		strcpy(parser->symbol, parser->currentCommand + 1);
		parser->symbol[strlen(parser->symbol) - 1] = '\0';
	}
	else
	{
		parser->commandType = C_COMMAND;
		parser->lineNumber++;
		char *equals = strchr(parser->currentCommand, '=');
		if (equals)
		{
			strncpy(parser->dest, parser->currentCommand, equals - parser->currentCommand);
			parser->dest[equals - parser->currentCommand] = '\0';
		}
		else
		{
			parser->dest[0] = '\0';
		}

		char *semicolon = strchr(parser->currentCommand, ';');
		if (semicolon)
		{
			strncpy(parser->comp, equals ? equals + 1 : parser->currentCommand, semicolon - (equals ? equals + 1 : parser->currentCommand));
			parser->comp[semicolon - (equals ? equals + 1 : parser->currentCommand)] = '\0';
			strncpy(parser->jump, semicolon + 1, sizeof(parser->jump) - 1);
			parser->jump[sizeof(parser->jump) - 1] = '\0';
		}
		else
		{
			strncpy(parser->comp, equals ? equals + 1 : parser->currentCommand, sizeof(parser->comp) - 1);
			parser->comp[sizeof(parser->comp) - 1] = '\0';
			parser->jump[0] = '\0';
		}
	}
}
