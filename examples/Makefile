CFLAGS = -Wall -Wextra -Werror -pedantic
CFLAGS_ALL = $(CFLAGS)

LDFLAGS =
LDFLAGS_ALL = $(LDFLAGS) -lsuinput

CPPFLAGS =
CPPFLAGS_ALL = $(CPPFLAGS)

PROGRAMS = keyboard mouse

.PHONY: all clean

all: $(PROGRAMS)

%.o : %.c
	$(CC) $(CPPFLAGS_ALL) $(CFLAGS_ALL) -c -o $@ $<

$(PROGRAMS) : % : %.o
	$(CC) $(LDFLAGS_ALL) $(CFLAGS_ALL) -o $@ $<

clean:
	rm -f $(PROGRAMS)
	rm -f *.o
