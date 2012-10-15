## 
## Simple makefile for decaf programming projects
##


.PHONY: clean strip all test

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

default: all

all: dcc

# Generate the lexer implementation
lex.yy.cpp: scanner.lpp parser.ypp y.tab.hpp parser.hpp
	$(LEX) $(LEXFLAGS) -o $@ scanner.lpp

# Generate the parser implementation
y.tab.hpp y.tab.cpp: parser.ypp parser.hpp
	$(YACC) $(YACCFLAGS) --defines=y.tab.hpp -o y.tab.cpp parser.ypp

# Object construction rules

OBJBUILD = $(CC) $(CFLAGS) -c -o $@ $*.cpp

lex.yy.o: lex.yy.cpp
	$(OBJBUILD)

y.tab.o: y.tab.cpp
	$(OBJBUILD)

ast.o: ast.cpp
	$(OBJBUILD)

ast_decl.o: ast_decl.cpp
	$(OBJBUILD)

ast_expr.o: ast_expr.cpp
	$(OBJBUILD)

ast_stmt.o: ast_stmt.cpp
	$(OBJBUILD)

ast_type.o: ast_type.cpp
	$(OBJBUILD)

errors.o: errors.cpp
	$(OBJBUILD)

main.o: main.cpp
	$(OBJBUILD)

utility.o: utility.cpp
	$(OBJBUILD)

scope.o: scope.cpp
	$(OBJBUILD)

# rules to build compiler (dcc)

DCC_OBJS = lex.yy.o   \
           y.tab.o    \
	   ast.o      \
	   ast_decl.o \
           ast_expr.o \
           ast_stmt.o \
           ast_type.o \
           errors.o   \
           main.o     \
           utility.o  \
           scope.o

dcc: $(DCC_OBJS)
	$(LD) -o $@ $(DCC_OBJS) $(LIBS)

# This target is to build small for testing (no debugging info), removes
# all intermediate products, too
strip : $(PRODUCTS)
	strip $(PRODUCTS)
	rm -rf $(JUNK)

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

JUNK = *.o lex.yy.cpp y.tab.cpp y.tab.hpp y.output
PRODUCTS = dcc
clean:
	rm -f $(JUNK) $(PRODUCTS)

test: dcc
	bash diff-all.sh ./ ./solution ./samples
# DO NOT DELETE
