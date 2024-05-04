# Isaac Boaz
# 05/03/24

CC=gcc
CFLAGS=-g -Wall
FILES=Simulation.c types.c
HEADERS=${FILE:.c=.h}
DERIV=${FILES:.c=.o}
DEPEND=$(DERIV)

all: Simulation
Simulation: $(DEPEND) $(HEADERS)
	$(CC) -o Simulation $(CFLAGS) $(DERIV)

clean:
	rm -f $(DERIV) Simulation

