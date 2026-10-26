CC = cc
CFLAGS = -O2 -Wall -Wextra -std=gnu11
PREFIX = /usr
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib

.PHONY: all test install clean
all: move libprotect.so safemove

move: move.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $<

libprotect.so: protect.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -shared -fPIC -o $@ $< -ldl

safemove: safemove.in Makefile
	sed -e 's|@BINDIR@|$(BINDIR)|g' -e 's|@LIBDIR@|$(LIBDIR)|g' $< > $@
	chmod +x $@

test: all
	sh test.sh

install: all
	install -Dm755 move $(DESTDIR)$(BINDIR)/move
	install -Dm755 safemove $(DESTDIR)$(BINDIR)/safemove
	install -Dm755 libprotect.so $(DESTDIR)$(LIBDIR)/libprotect.so

clean:
	rm -f move libprotect.so safemove
