#
# Type 'make' with this 'makefile' file to build the example BLITZ programs.
# It will execute the following commands as needed, based on files'
# most-recent-update times.
# 

all: Synch

Synch.s: Synch.h Synch.k
	kpl Synch -unsafe

Synch.o: Synch.s
	asm Synch.s

Synch: Synch.o
	lddd Synch.o -o Synch

Runtime.o: Runtime.s
	asm Runtime.s

System.s: System.h System.k
	kpl System -unsafe

System.o: System.s
	asm System.s

clean:
	rm -f Runtime.o System.s System.o Synch.o Synch.s Synch

