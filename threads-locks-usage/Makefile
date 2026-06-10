
FLAGS = -Wall -pthread
INCLUDES = ../include

all: counter approx-counter list list-hoh hash hash-hoh

clean:
	rm -f counter approx-counter list list-hoh hash hash-hoh

counter: counter.c
	gcc -I $(INCLUDES) -o counter counter.c $(FLAGS)

approx-counter: approx-counter.c
	gcc -I $(INCLUDES) -o approx-counter approx-counter.c $(FLAGS)

list: list.c
	gcc -I $(INCLUDES) -o list list.c $(FLAGS)

list-hoh: list-hoh.c
	gcc -I $(INCLUDES) -o list-hoh list-hoh.c $(FLAGS)

hash: hash.c
	gcc -I $(INCLUDES) -o hash hash.c $(FLAGS)

hash-hoh: hash-hoh.c
	gcc -I $(INCLUDES) -o hash-hoh hash-hoh.c $(FLAGS)