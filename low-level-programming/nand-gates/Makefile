GCC      = gcc
FLAGS    = -Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2
LDFLAGS  = -shared -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

.PHONY: all clean

all: libnand.so

%.o: %.c
	$(GCC) $(FLAGS) -c $< -o $@

libnand.so: nand.o
	$(GCC) $(LDFLAGS) $^ -o $@

nand.o: nand.c nand.h

clean:
	rm -rf libnand.so *.o
