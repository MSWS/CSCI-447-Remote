# Isaac Boaz
# 05/10/24

CC=gcc
CFLAGS=-g -Wall
FILES=Simulation.c process.c queue.c common.c
DERIV=${FILES:.c=.o}
DEPEND=$(DERIV)

all: Simulation
Simulation: $(DEPEND)
	$(CC) -o Simulation $(CFLAGS) $(DERIV)

clean:
	rm -f $(DERIV) Simulation

test: test.sh all
	./test.sh
	rm unit.txt
