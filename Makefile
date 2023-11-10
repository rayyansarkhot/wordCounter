CC = gcc
CFLAGS = -std=c99 -Wall -fsanitize=address,undefined

SRCS = wordCounter.c bst.c
EXE = wordCounter

all: $(EXE)

$(EXE): $(SRCS:.c=.o)
	$(CC) -o $(EXE) $(SRCS:.c=.o) $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(EXE) $(SRCS:.c=.o)
