/* File: scanner.h
 * ---------------
 * You should not need to modify this file. It declare a few constants,
 * types, variables,and functions that are used and/or exported by
 * the lex-generated scanner.
 */

#ifndef _H_scanner
#define _H_scanner

#include <string>
using std::string;

#include <stdio.h>

#define MaxIdentLen 31    // Maximum length for identifiers

extern char *yytext;      // Text of lexeme just scanned


int yylex();              // Defined in the generated lex.yy.c file
void yyrestart(FILE *fp); // ditto


void InitScanner();                 // Defined in scanner.l user subroutines
const string& GetLineNumbered(int n); // ditto
 
#endif
