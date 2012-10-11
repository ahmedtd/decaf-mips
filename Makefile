## 
## Simple makefile for decaf programming projects
##


.PHONY: clean strip

# Set the default target. When you make with no arguments,
# this will be the target built.
COMPILER = dcc
PRODUCTS = $(COMPILER) 
default: $(PRODUCTS)

# Set up the list of source and object files
SRCS = ast.cc ast_decl.cc ast_expr.cc ast_stmt.cc ast_type.cc errors.cc utility.cc main.cc \


# OBJS can deal with either .cc or .c files listed in SRCS
OBJS = y.tab.o lex.yy.o $(patsubst %.cc, %.o, $(filter %.cc,$(SRCS))) $(patsubst %.c, %.o, $(filter %.c, $(SRCS)))

JUNK =  *.o lex.yy.c dpp.yy.c y.tab.c y.tab.h *.core core $(COMPILER).purify purify.log 

# Define the tools we are going to use
#CC= g++-4.6
#LD = g++-4.6
CC = g++
LD = g++

LEX = flex
YACC = bison

# Set up the necessary flags for the tools

# We want debugging and most warnings, but lex/yacc generate some
# static symbols we don't use, so turn off unused warnings to avoid clutter
# Also STL has some signed/unsigned comparisons we want to suppress
CFLAGS = -g -Wall -Wno-unused -Wno-sign-compare --pedantic -std=c++0x

# The -d flag tells lex to set up for debugging. Can turn on/off by
# setting value of global yy_flex_debug inside the scanner itself
LEXFLAGS = -d

# The -d flag tells yacc to generate header with token types
# The -v flag writes out a verbose description of the states and conflicts
# The -t flag turns on debugging capability
# The -y flag means imitate yacc's output file naming conventions
YACCFLAGS = -dvty

# Link with standard c library, math library, and lex library
#LIBS = -lc -lm -ll
LIBS = -lc -lm -lfl

# Rules for various parts of the target

.yy.o: $*.yy.c
	$(CC) $(CFLAGS) -c -o $@ $*.cc

lex.yy.c: scanner.l  parser.y y.tab.h 
	$(LEX) $(LEXFLAGS) scanner.l

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c -o y.tab.o y.tab.c

y.tab.h y.tab.c: parser.y
	$(YACC) $(YACCFLAGS) parser.y
.cc.o: $*.cc
	$(CC) $(CFLAGS) -c -o $@ $*.cc

# rules to build compiler (dcc)

dcc:  $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

# This target is to build small for testing (no debugging info), removes
# all intermediate products, too
strip : $(PRODUCTS)
	strip $(PRODUCTS)
	rm -rf $(JUNK)


# make depend will set up the header file dependencies for the 
# assignment.  You should make depend whenever you add a new header
# file to the project or move the project between machines
#
depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f $(JUNK) y.output $(PRODUCTS)

test: dcc
	bash diff-all.sh ./ ./solution ./samples
# DO NOT DELETE

ast.o: ast.h /usr/include/stdlib.h /usr/include/features.h
ast.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
ast.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
ast.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
ast.o: /usr/include/endian.h /usr/include/bits/endian.h
ast.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
ast.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
ast.o: /usr/include/time.h /usr/include/sys/select.h
ast.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
ast.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
ast.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h location.h
ast.o: ast_type.h ast_decl.h /usr/include/string.h /usr/include/xlocale.h
ast.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
ast.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
ast.o: /usr/include/bits/sys_errlist.h
ast_decl.o: ast_decl.h ast.h /usr/include/stdlib.h /usr/include/features.h
ast_decl.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
ast_decl.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
ast_decl.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
ast_decl.o: /usr/include/endian.h /usr/include/bits/endian.h
ast_decl.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
ast_decl.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
ast_decl.o: /usr/include/time.h /usr/include/sys/select.h
ast_decl.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
ast_decl.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
ast_decl.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h location.h
ast_decl.o: ast_type.h ast_stmt.h
ast_expr.o: ast_expr.h ast.h /usr/include/stdlib.h /usr/include/features.h
ast_expr.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
ast_expr.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
ast_expr.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
ast_expr.o: /usr/include/endian.h /usr/include/bits/endian.h
ast_expr.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
ast_expr.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
ast_expr.o: /usr/include/time.h /usr/include/sys/select.h
ast_expr.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
ast_expr.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
ast_expr.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h location.h
ast_expr.o: ast_stmt.h ast_type.h ast_decl.h /usr/include/string.h
ast_expr.o: /usr/include/xlocale.h
ast_stmt.o: ast_stmt.h ast.h /usr/include/stdlib.h /usr/include/features.h
ast_stmt.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
ast_stmt.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
ast_stmt.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
ast_stmt.o: /usr/include/endian.h /usr/include/bits/endian.h
ast_stmt.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
ast_stmt.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
ast_stmt.o: /usr/include/time.h /usr/include/sys/select.h
ast_stmt.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
ast_stmt.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
ast_stmt.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h location.h
ast_stmt.o: ast_type.h ast_decl.h ast_expr.h
ast_type.o: ast_type.h ast.h /usr/include/stdlib.h /usr/include/features.h
ast_type.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
ast_type.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
ast_type.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
ast_type.o: /usr/include/endian.h /usr/include/bits/endian.h
ast_type.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
ast_type.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
ast_type.o: /usr/include/time.h /usr/include/sys/select.h
ast_type.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
ast_type.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
ast_type.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h location.h
ast_type.o: ast_decl.h /usr/include/string.h /usr/include/xlocale.h
errors.o: errors.h location.h /usr/include/stdio.h /usr/include/features.h
errors.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
errors.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
errors.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
errors.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
errors.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
errors.o: scanner.h ast_type.h ast.h /usr/include/stdlib.h
errors.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
errors.o: /usr/include/endian.h /usr/include/bits/endian.h
errors.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
errors.o: /usr/include/time.h /usr/include/sys/select.h
errors.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
errors.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
errors.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h ast_expr.h
errors.o: ast_stmt.h ast_decl.h
utility.o: utility.h /usr/include/stdlib.h /usr/include/features.h
utility.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
utility.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
utility.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
utility.o: /usr/include/endian.h /usr/include/bits/endian.h
utility.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
utility.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
utility.o: /usr/include/time.h /usr/include/sys/select.h
utility.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
utility.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
utility.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
utility.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
utility.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
utility.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
utility.o: /usr/include/xlocale.h
main.o: /usr/include/string.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
main.o: /usr/include/xlocale.h /usr/include/stdio.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: utility.h /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/sys/types.h /usr/include/time.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
main.o: /usr/include/alloca.h errors.h location.h parser.h scanner.h ast.h
main.o: ast_type.h ast_decl.h ast_expr.h ast_stmt.h y.tab.h
