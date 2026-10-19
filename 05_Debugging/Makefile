CC = cc
CFLAGS = -Wall -O0 -g
GDB = gdb

all: range

range: range.c
	$(CC) $(CFLAGS) $< -o $@

test: range
	$(GDB) --batch --quiet --nx -x test1.gdb > test1.log
	grep '^@@@' test1.log > test1.out
	diff -u test1.expected test1.out
	$(GDB) --batch --quiet --nx -x test2.gdb > test2.log
	grep '^@@@' test2.log > test2.out
	diff -u test2.expected test2.out

clean:
	rm -f range *.o test1.log test2.log test1.out test2.out

.PHONY: all test clean
