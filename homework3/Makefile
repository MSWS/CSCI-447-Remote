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

test: test_simple
	rm unit.txt

test_hw: all
	./Simulation tests/hw/hw.txt 0 3 0 > unit.txt
	diff tests/hw/X30.txt unit.txt
	./Simulation tests/hw/hw.txt 0 3 1 > unit.txt
	diff tests/hw/X31.txt unit.txt

test_simple: all
	./Simulation tests/simple_1/simple_1.txt 0 3 0 > unit.txt
	diff tests/simple_1/X30.txt unit.txt
	./Simulation tests/simple_1/simple_1.txt 0 3 1 > unit.txt
	diff tests/simple_1/X31.txt unit.txt
	./Simulation tests/simple_1/simple_1.txt 0 5 0 > unit.txt
	diff tests/simple_1/X50.txt unit.txt
	./Simulation tests/simple_1/simple_1.txt 0 5 1 > unit.txt
	diff tests/simple_1/X51.txt unit.txt

