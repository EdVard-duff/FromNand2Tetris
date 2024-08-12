#include "code.h"
#include "symbolTable.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *mnemonic;
    const char *binary;
} TableEntry;

static const TableEntry compTable[] = {
    {"0", "0101010"}, {"1", "0111111"}, {"-1", "0111010"},
    {"D", "0001100"}, {"A", "0110000"}, {"M", "1110000"},
    {"!D", "0001101"}, {"!A", "0110001"}, {"!M", "1110001"},
    {"-D", "0001111"}, {"-A", "0110011"}, {"-M", "1110011"},
    {"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"},
    {"D-1", "0001110"}, {"A-1", "0110010"}, {"M-1", "1110010"},
    {"D+A", "0000010"}, {"D+M", "1000010"}, {"D-A", "0010011"},
    {"D-M", "1010011"}, {"A-D", "0000111"}, {"M-D", "1000111"},
    {"D&A", "0000000"}, {"D&M", "1000000"}, {"D|A", "0010101"},
    {"D|M", "1010101"}
};

static const TableEntry jumpTable[] = {
    {"", "000"}, {"JGT", "001"}, {"JEQ", "010"}, 
	{"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"}, 
	{"JLE", "110"}, {"JMP", "111"}
};

// Helper function to lookup in the table
const char* lookupBinary(const char *mnemonic, const TableEntry table[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (strcmp(mnemonic, table[i].mnemonic) == 0) {
            return table[i].binary;
        }
    }
    return "0000000"; // Default value for unknown mnemonic
}

// Helper functions to convert mnemonics to binary
void intToBinary(int n, char *binaryStr) {
    binaryStr[0] = '0';

    for (int i = 0; i <= 14; i++) {
        binaryStr[15 - i] = (n & (1 << i)) ? '1' : '0';
    }  

    binaryStr[16] = '\0';
}

char* dest(const char* mnemonic, char* dest) {
    memset(dest, '0', 3);
    if (strchr(mnemonic, 'A')) {
        dest[A_BIT] = '1';
    }
    if (strchr(mnemonic, 'D')) {
        dest[D_BIT] = '1';
    }
    if (strchr(mnemonic, 'M')) {
        dest[M_BIT] = '1';
    }
    dest[3] = '\0';
    return dest;
}

char* comp(const char* mnemonic, char* dest) {
	const char *binary = lookupBinary(mnemonic, compTable, COMP_SIZE);
	strcpy(dest, binary);
	return dest;
}

char* jump(const char* mnemonic, char* dest) {
	const char *binary = lookupBinary(mnemonic, jumpTable, JUMP_SIZE);
	strcpy(dest, binary);
	return dest;
}

char* getBinaryCode(Parser *parser, char *binaryCode) {
	if (parser->commandType == A_COMMAND) { // A-instruction
		if (isdigit(parser->currentCommand[1])) { // Decimal number
			intToBinary(parser->address, binaryCode);
		} 
        else { // Symbol
			int address = findSymbol(parser->symbol);
			if (address == -1) {
				addEntry(parser->symbol, parser->heapAddress);
				address = parser->heapAddress;
				parser->heapAddress++;
			}
			intToBinary(address, binaryCode);
		} 
	} 
    else if (parser->commandType == C_COMMAND) { // C-instruction
        // Write the fixed prefix for C-instructions
        strcpy(binaryCode, "111"); 
        binaryCode = comp(parser->comp, binaryCode + 3); 
        binaryCode = dest(parser->dest, binaryCode + 7);
        binaryCode = jump(parser->jump, binaryCode + 3);
		binaryCode[3] = '\0';
	} 
    else {
		return NULL;
	}

	return binaryCode;
}
