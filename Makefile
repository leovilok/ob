LDLIBS ?= -lSDL2  -lSDL2_image
CFLAGS ?= -Wall -Wextra

all: test

test: core.o clib.c
