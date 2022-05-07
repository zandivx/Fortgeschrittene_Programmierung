CC            := gcc
CCFLAGS       := -Wall -Wextra -pedantic -std=c17 -g -c
LDFLAGS       := -lm
OUTPUT        := a.out

BUILDDIR      := build
SOURCES       := $(wildcard *.c)
OBJECTS       := $(patsubst %,$(BUILDDIR)/%,${SOURCES:.c=.o})

.PHONY: clean

$(OUTPUT): $(OBJECTS)
    $(CC) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.c
    @mkdir -p $(BUILDDIR)
    $(CC) $(CCFLAGS) -o $@ -MMD -MF ./$@.d $<

clean:
    rm -rf $(BUILDDIR)
    rm -f $(OUTPUT)

-include $(wildcard *.d)