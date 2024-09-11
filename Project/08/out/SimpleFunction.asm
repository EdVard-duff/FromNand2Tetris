// function SimpleFunction.test 2
(SimpleFunction.test)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// push local 0
@1
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 1
@1
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
AM=M-1
D=M
@SP
AM=M-1
M=D+M
@SP
M=M+1

// not
@SP
AM=M-1
M=!M
@SP
M=M+1

// push argument 0
@2
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
AM=M-1
D=M
@SP
AM=M-1
M=D+M
@SP
M=M+1

// push argument 1
@2
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// sub
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M-D
@SP
M=M+1

// return
@LCL
D=M
@R14
M=D
@5
A=D-A
D=M
@R13
M=D
@2
D=M
@0
D=D+A
@R15
M=D
@SP
AM=M-1
D=M
@R15
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R14
A=M-1
AD=M
@THAT
M=D
@R14
D=M
@2
A=D-A
AD=M
@THIS
M=D
@R14
D=M
@3
A=D-A
AD=M
@ARG
M=D
@R14
D=M
@4
A=D-A
AD=M
@LCL
M=D
@R13
A=M
0;JMP

