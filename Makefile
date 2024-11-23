# C compiler
CLANGFLAGS = -fsyntax-only -Ofast -std=c99 -Werror -Weverything -Wno-poison-system-directories

CFLAGS = -std=c99
CFLAGS += -Wall -Wextra -Wpedantic -Wconversion -Wdouble-promotion -Wnull-dereference -Wignored-attributes -Wattributes -Wswitch-default -Wfloat-equal -Wshadow -Wunused-macros -Wdangling-else -Wstrict-prototypes -Wpadded -Wredundant-decls -Wnested-externs -Winline -Werror
CFLAGS += -Ofast
# CFLAGS += -g -O0

EXECS = cl

.PHONY: all clean run debug

all: $(EXECS)

% :: %.c
	clang $(CLANGFLAGS) $<
	gcc $< $(CFLAGS) -o $@

clean:
	rm -vf $(EXECS)
	rm -rvf *.dSYM

run:
	./utilities/run_curr_binary
