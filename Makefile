CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -g

OBJS    = main.o filesystem.o storage.o block_manager.o directory.o file_operations.o
TARGET  = sfs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c filesystem.h
	$(CC) $(CFLAGS) -c main.c

filesystem.o: filesystem.c filesystem.h storage.h block_manager.h directory.h file_operations.h
	$(CC) $(CFLAGS) -c filesystem.c

storage.o: storage.c storage.h filesystem.h
	$(CC) $(CFLAGS) -c storage.c

block_manager.o: block_manager.c block_manager.h filesystem.h
	$(CC) $(CFLAGS) -c block_manager.c

directory.o: directory.c directory.h filesystem.h
	$(CC) $(CFLAGS) -c directory.c

file_operations.o: file_operations.c file_operations.h filesystem.h directory.h block_manager.h storage.h
	$(CC) $(CFLAGS) -c file_operations.c

clean:
	rm -f $(OBJS) $(TARGET)
