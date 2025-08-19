CC = gcc
CFLAGS = -Wall -Werror
LIB_TARGET = lib

CDFLAGS = -pg -g
CRFLAGS = -O3

.PHONY: all debug release test clean
all: bum_tschak

debug: CFLAGS += $(CDFLAGS)
debug: LIB_TARGET = lib_debug
debug: bum_tschak
release: CFLAGS += $(CRFLAGS)
release: LIB_TARGET = lib_release
release: bum_tschak

bum_tschak: bum_tschak.o not_enough_cli/bin/lib.o
	$(CC) $(CFLAGS) $^ -o $@

not_enough_cli/bin/lib.o:
	@cd not_enough_cli && make $(LIB_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: bum_tschak
	@cd test && make
	# @echo "test/TODO generated"

clean:
	rm -f *.o bum_tschak
	@cd test && make clean
	@cd not_enough_cli && make clean
