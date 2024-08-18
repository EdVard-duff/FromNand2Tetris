CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -I.

TARGET = VMTranslator
OBJS = utility.o parser.o

all: $(TARGET)

$(TARGET): main.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c utility.h parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

parser.o: parser.c parser.h utility.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean