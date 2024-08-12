// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// Initialize target and counter
@target
M=0             // Set target to 0

@i
M=0             // Initialize counter i to 0

@24575
D=A
@screenEnd
M=D             // screenEnd is set to 24575 (end of screen memory)

(INFINITE_LOOP)
    @KBD
    D=M

    // If the keyboard is pressed, set the target to -1, otherwise set it to 0
    @LIGHT_UP
    D;JNE
    @target
    M=0

    @FILL_SCREEN
    0;JMP

(FILL_SCREEN)
    @SCREEN
    D=A
    @i
    M=D

(LOOP)
    @i
    D=M
    @screenEnd
    D=D-M

    @INFINITE_LOOP
    D;JEQ

    @target
    D=M
    @i
    A=M
    M=D  // Set memory at address i to target

    @i
    M=M+1 // Increment i

    @LOOP
    0;JMP

(LIGHT_UP)
    @target
    M=-1         // Set target to -1 if any key is pressed
    @FILL_SCREEN
    0;JMP        // Jump to FILL_SCREEN to update the screen