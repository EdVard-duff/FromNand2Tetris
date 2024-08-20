CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -I. # 指定头文件的路径

TARGET = VMTranslator
OBJS = parser.o codeWriter.o utility.o 

all: $(TARGET)

$(TARGET): main.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c # 编译器会自动找到 .c 文件中依赖的 .h 文件. 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean