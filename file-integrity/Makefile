CC = clang
CFLAGS = -Wall -Wextra

all: check-xor.out check-fletcher.out

%.out: %.c common.h
	$(CC) -o $@ $< $(CFLAGS)