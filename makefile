CC = gcc
CFLAGS = -Wall

TARGETS = shell test

default: shell

all: $(TARGETS)

shell: shell.o dirutils.o parsing.o shutility.o hcommand.o
	$(CC) $(CFLAGS) shell.o dirutils.o parsing.o shutility.o hcommand.o -o shell
	
test: test.o
	$(CC) $(CFLAGS) test.o -o test

clean:
	rm -f *.o *~ a.out *.txt $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<

