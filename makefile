CC = gcc
CFLAGS = -Wall

TARGETS = shell_sim shell

default: shell

all: $(TARGETS)

shell_sim: shell_sim.o
	$(CC) $(CFLAGS) shell_sim.o -o shell_sim

shell: shell.o dirutils.o
	$(CC) $(CFLAGS) shell.o dirutils.o -o shell

clean:
	rm -f *.o *~ a.out $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<

