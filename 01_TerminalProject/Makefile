CC = cc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lncurses

PREFIX = /usr
BINDIR = $(PREFIX)/bin
DESTDIR =

Show: Show.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

install: Show
	install -D -m755 Show "$(DESTDIR)$(BINDIR)/Show"

clean:
	rm -f Show

.PHONY: clean
