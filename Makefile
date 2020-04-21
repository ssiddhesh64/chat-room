CC=g++
CFLAGS = -c -Wall
LIBS = -pthread

TARGETS=client1 client2
# OBJ=client1.o client2.o

all: $(TARGETS)

# $(TARGETS):$(OBJ)
# 	$(CC) -o $@  $% ${LIBS}

client1: client1.o
	$(CC) $^ -o $@ ${LIBS}

client2: client2.o
	$(CC) $^ -o $@ ${LIBS}

# $(OBJ):

# %.o: 

.PHONY: clean

clean:
	rm -r *.o client1 client2
	clear
	