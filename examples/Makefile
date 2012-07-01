CFLAGS=-Wall -Wextra -Werror -pedantic -std=c89
LDFLAGS=-lsuinput
PROGRAMS=keyboard mouse

.PHONY: all clean

all: $(PROGRAMS)

%: %.c
	gcc -o$@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -f $(PROGRAMS)
