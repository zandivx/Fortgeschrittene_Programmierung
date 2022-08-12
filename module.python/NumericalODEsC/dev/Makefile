CC            := gcc
CCFLAGS       := -Wall -Wextra -pedantic -std=c17 -ggdb3 -c  # -g: debugging (with macro info for gdb) -c: compile only, no linking
LDFLAGS       := -lm  # linker flag
OUTPUT        := a.out

BUILDDIR      := build
SOURCES       := $(wildcard *.c)
OBJECTS       := $(patsubst %,$(BUILDDIR)/%,${SOURCES:.c=.o})

.PHONY: clean

$(OUTPUT): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CCFLAGS) -o $@ -MMD -MF ./$@.d $<

clean:
	rm -rf $(BUILDDIR)
	rm -f $(OUTPUT)

-include $(wildcard *.d)