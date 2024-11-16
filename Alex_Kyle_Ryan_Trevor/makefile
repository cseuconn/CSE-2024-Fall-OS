CC = gcc
CFLAGS = -Wall -g
OBJ = main.o process.o scheduler.o
TARGET = Scheduler_App

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c process.h scheduler.h
	$(CC) $(CFLAGS) -c main.c

process.o: process.c process.h
	$(CC) $(CFLAGS) -c process.c

scheduler.o: scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c scheduler.c

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean