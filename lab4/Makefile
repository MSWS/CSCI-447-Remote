# declares compiler command and flags
CC=gcc
CFLAGS=-g -Wall

#declares files to compile
FILES = PageSim.c Simulate.c

#declare header files
DEPEND = $(DERIV) MemSim.h PageSim.h

#changes files from .c to .o
DERIV =${FILES:.c=.o}

Simulation: $(DEPEND)
	$(CC) -o Simulation $(CFLAGS) $(DERIV)

clean:
	rm -f $(DERIV) run
