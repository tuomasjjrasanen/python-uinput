CFLAGS=-Wall -Wextra -Werror -pedantic -std=c89
LDFLAGS=
PROGRAMS=kbd mouse

.PHONY: all clean

all: $(PROGRAMS)

%: %.c
	gcc -o$@ $(CFLAGS) $(LDFLAGS) -lsuinput $^

clean:
	rm -f $(PROGRAMS)
