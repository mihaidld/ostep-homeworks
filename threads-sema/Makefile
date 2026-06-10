CC     := gcc
CFLAGS := -Wall -Werror -pthread

OS     := $(shell uname -s)
LIBS   := 
ifeq ($(OS),Linux)
	LIBS += -pthread
endif

SRCS   := fork-join.c \
	rendezvous.c \
	barrier.c \
	reader-writer.c \
	reader-writer-nostarve.c \
	mutex-nostarve.c
	
	
	

OBJS   := ${SRCS:c=o}
PROGS  := ${SRCS:.c=}

.PHONY: all
all: ${PROGS}

${PROGS} : % : %.o Makefile
	${CC} $< -o $@ ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS}

%.o: %.c Makefile
	${CC} ${CFLAGS} -c $<
