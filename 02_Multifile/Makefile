GENERATES = prog prog-a prog-so README liboutput_static.a liboutput.so *.txt
TRASH = *.o *~ o.*

%.o:    %.c
		cc -fPIC  $< -c -o $@

all:    README prog prog-a prog-so

prog:   const.o fun.o prog.o
		cc $^ -o $@

prog-so: prog.o liboutput.so
		cc -L. $^ -o $@

prog-a: prog.o liboutput_static.a
		cc -L. $^ -o $@

liboutput.so: fun.o const.o
		cc -shared $^ -o $@

liboutput_static.a: fun.o const.o
		ar -rcs $@ $^

README: prog
		./$< 2> $@

test: prog prog-a prog-so
	./prog > test_prog_0.txt 2>&1
	./prog test1 > test_prog_1.txt 2>&1
	./prog test1 test2 test3 > test_prog_3.txt 2>&1
	./prog-a > test_prog_a_0.txt 2>&1
	./prog-a test1 > test_prog_a_1.txt 2>&1
	./prog-a test1 test2 test3 > test_prog_a_3.txt 2>&1
	LD_LIBRARY_PATH=`pwd` ./prog-so > test_prog_o_0.txt 2>&1
	LD_LIBRARY_PATH=`pwd` ./prog-so test1 > test_prog_o_1.txt 2>&1
	LD_LIBRARY_PATH=`pwd` ./prog-so test1 test2 test3 > test_prog_o_3.txt 2>&1
	cmp test_prog_0.txt test_prog_a_0.txt
	cmp test_prog_1.txt test_prog_a_1.txt
	cmp test_prog_3.txt test_prog_a_3.txt
	cmp test_prog_0.txt test_prog_o_0.txt
	cmp test_prog_1.txt test_prog_o_1.txt
	cmp test_prog_3.txt test_prog_o_3.txt


clean:
		rm -f $(TRASH)
		rm -rf $(GENERATES)
