all: unicon

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
OPTS = -lm

unicon: Makefile unicon.c
	$(CC) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) $(OPTS) unicon.c

clean:
	rm -f unicon

install:
	echo "Installing is not supported"

run:
	./unicon

