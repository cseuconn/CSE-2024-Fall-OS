CC = gcc
CFLAGS = -Wall -g
OBJ = main.o process.o scheduler.o
TARGET = scheduler_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c process.h scheduler.h
	$(CC) $(CFLAGS) -c main.c

process.o: process.h
	touch process.o

scheduler.o: scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c scheduler.c

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean