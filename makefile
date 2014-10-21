CC = gcc

CPPFLAGS= `pkg-config --cflags gtk+-3.0`
CFLAGS= -std=c99

LDLIBS= `pkg-config --libs gtk+-3.0` -rdynamic

SRC= main.c callbacks.c 
OBJ= ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	rm -f *~ *.o
	rm -f main

# END

