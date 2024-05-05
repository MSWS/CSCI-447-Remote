# Isaac Boaz
# 05/03/24

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

test: all
	./Simulation sampleFile.txt 7 3 0
