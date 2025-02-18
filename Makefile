CC=gcc -std=c11
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-lncurses

all: pong pong_interactive

pong: pong.o pong_common.o
	$(CC) $(CFLAGS) $^ -o $@

pong_interactive: pong_interactive.o pong_common.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o pong pong_interactive

rebuild: clean all
