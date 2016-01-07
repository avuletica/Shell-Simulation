CCO = gcc -o
CC1 = gcc -c
CFLAGS = -Wall

TARGETS = shell test

default: shell

all: $(TARGETS)

shell: shell.o dirutils.o parsing.o shutility.o hcommand.o
	$(CCO) $(CFLAGS) shell.o dirutils.o parsing.o shutility.o hcommand.o -o shell

shell.o: shell.c parsing.c parsing.h shutility.c shutility.h
	$(CC1) $(CFLAGS) main.c

dirutils.o: dirutils.c dirutils.h
	$(CC1) $(CFLAGS) dirutils.c

parsing.o: parsing.c parsing.h shutility.c shutility.h hcommand.c hcommand.h
	$(CC1) $(CFLAGS) dirutils.c parsing.c shutility.c hcommand.c

shutility.o: shutility.c shutility.h
	$(CC1) $(CFLAGS) shutility.c

hcommand.o: hcommand.c hcommand.h dirutils.c dirutils.h shutility.c shutility.h
	$(CC1) $(CFLAGS) hcommand.c dirutils.c shutility.c

test: test.o
	$(CCO) $(CFLAGS) test.o -o test

clean:
	rm -f *.o *~ a.out *.txt $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<