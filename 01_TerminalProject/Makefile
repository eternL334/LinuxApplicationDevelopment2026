CC = cc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lncurses

Show: Show.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f Show

.PHONY: clean
