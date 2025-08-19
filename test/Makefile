CC = gcc
CFLAGS = -Wall -Werror

CDFLAGS = -pg -g
CRFLAGS = -O3

.PHONY: all debug release test clean
all: bum_tschak_test

debug: CFLAGS += $(CDFLAGS)
debug: bum_tschak_test
release: CFLAGS += $(CRFLAGS)
release: bum_tschak_test

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o bum_tschak_test
