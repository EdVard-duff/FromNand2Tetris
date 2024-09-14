CC = gcc

# 使用 pkg-config 获取 glib 的编译和链接选项
PKG_CONFIG = pkg-config
GLIB_CFLAGS = $(shell $(PKG_CONFIG) --cflags glib-2.0)
GLIB_LIBS = $(shell $(PKG_CONFIG) --libs glib-2.0)

DEFINE_FLAGS =  # -D选项用于定义宏. 
COMPILE_FLAGS = -Wall -g -O3 $(DEFINE_FLAGS) $(GLIB_CFLAGS) # 指定编译选项
CPPFLAGS = -I. # 指定头文件的路径
LINK_FLAGS = $(GLIB_LIBS) # 指定链接选项

TARGET = JackAnalyzer
OBJS = utility.o tokenizer.o compilerEngine.o symbolTable.o codeWriter.o

OUT_DIR = out

all: $(TARGET) $(OUT_DIR)

$(TARGET): main.o $(OBJS)
	$(CC) $(COMPILE_FLAGS) $(CPPFLAGS) -o $@ $^ $(LINK_FLAGS)

%.o: %.c
	$(CC) $(COMPILE_FLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
	@echo "Directory '$(OUT_DIR)' created."

.PHONY: all clean
