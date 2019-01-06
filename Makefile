# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -g
# name for executable
EXE = compiler

# space-separated list of header files
HDRS = helpers.h lexer.h builtin_functions.h codegen.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS =

# space-separated list of source files
SRCS = helpers.c lexer.c builtin_functions.c codegen.c compiler.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)

# default target
$(EXE): $(OBJS) $(HDRS) Makefile
		$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
		rm -f core $(EXE) *.o
