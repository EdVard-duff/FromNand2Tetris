// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// Goal: sum = a * b
// a = R0, b = R1, R2 = sum = 0
// for (mask=1; mask<=2^15; mask*=2) 
//      if  mask & b
//          sum += a;
//      a *= 2;
//      mask *= 2

@R0
D=M
@a 
M=D

@R1
D=M
@b
M=D

// R2 = 0
@R2
M=0

@mask
M=1

@16384 // 2^14
D=A
@maxMask
M=D 

(LOOP)
    @maxMask
    D=M
    @mask
    D=D-M

    @END // if mask > maxMask then END
    D;JEQ

    @mask
    D=M
    @b
    D=D&M
    
    @ADD // if mask & b then ADD 
    D;JNE 

    (BACKLOOP)
    // a *= 2
    @a
    D=M
    M=D+M

    // mask *= 2
    @mask
    D=M
    M=D+M

    @LOOP
    0;JMP   

(ADD)
    @a
    D=M
    @R2
    M=D+M
    @BACKLOOP // 加法结束后回到 LOOP 中
    0;JMP

(END)
    @END
    0;JMP 