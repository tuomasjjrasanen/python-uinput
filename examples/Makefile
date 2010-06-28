PROGRAMS=kbd mouse

.PHONY: all clean

all: $(PROGRAMS)

%: %.c
	gcc -o$@ -lsuinput -Wall $^

clean: $(PROGRAMS)
	rm -f $^
