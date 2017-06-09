LDLIBS ?= -lSDL2  -lSDL2_image -ldl
CFLAGS ?= -Wall -Wextra -fPIC

all: ob clib.so

%.so: %.o
	$(CC) $(LDFLAGS) -shared -o $@ $^ $(LDLIBS)

ob: ob.o

clib.so:

clean:
	-$(RM) *.o *.so ob
