CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -I.

# 目标文件
TARGET = Hackassembler
OBJS = parser.o symbolTable.o code.o

# 测试目标
TestTarget = test

all: $(TARGET) $(TestTarget)

# 生成主目标
$(TARGET): main.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 生成测试目标
$(TestTarget): test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c parser.h symbolTable.h code.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

symbolTable.o: symbolTable.c symbolTable.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

code.o: code.c code.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

test.o: test.c parser.h symbolTable.h code.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(TestTarget) $(OBJS) main.o test.o *.hack

.PHONY: all clean
