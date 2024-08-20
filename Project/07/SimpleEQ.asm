// It's a simple asm file that implements "eq" operation of the Jack Virtual Machine Language. This file is written for reference purposes.
// RAM[SP-2] = RAM[SP-2] == RAM[SP-1] ? -1 : 0

// SP--
@SP
AM=M-1

// Get the first operand
D=M

// SP--
@SP
AM=M-1

// Get the difference of the operands
D=M-D

@EQ_TRUE_i
D;JEQ
D=0
@EQ_CONT_i
0;JMP

(EQ_TRUE_i)
D=-1

// Set the result
(EQ_CONT_i)
@SP
A=M
M=D

// SP++
@SP
M=M+1



