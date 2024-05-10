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

test: test_faq
	rm unit.txt

test_faq: all
	./Simulation tests/faq_promotion.txt 5 10 0 > unit.txt
	diff tests/faq_promotion_exp.txt unit.txt
	./Simulation tests/faq_promotion.txt 5 10 1 > unit.txt
	diff tests/faq_promotion_exp_pre.txt unit.txt
test_homework: all
	./Simulation tests/homework.txt 7 3 0 > unit.txt
	diff tests/homework_exp.txt unit.txt
	./Simulation tests/homework.txt 7 3 1 > unit.txt
	diff tests/homework_exp.txt unit.txt
