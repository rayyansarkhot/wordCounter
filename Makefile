CC = gcc
CFLAGS = -std=c99 -Wall -fsanitize=address,undefined

SRCS = wordCounter.c bst.c
OBJS = $(SRCS:.c=.o)
EXE = wordCounter

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

# Assuming bst.h is a dependency for your source files
wordCounter.o: wordCounter.c bst.h
bst.o: bst.c bst.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXE) $(OBJS)