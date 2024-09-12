CC = gcc
DFLAGS =  # -D选项用于定义宏. 
CFLAGS = -Wall -g $(DFLAGS) 
CPPFLAGS = -I. # 指定头文件的路径

TARGET = JackAnalyzer
OBJS = utility.o tokenizer.o compilerEngine.o

OUT_DIR = out

all: $(TARGET) $(OUT_DIR)

$(TARGET): main.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c # 编译器会自动找到 .c 文件中依赖的 .h 文件. 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

$(OUT_DIR): 
	mkdir -p $(OUT_DIR)
	@echo "Directory '$(OUT_DIR)' created."

.PHONY: all clean