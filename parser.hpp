/* File: parser.h
 * --------------
 * This file provides constants and type definitions that will
 * are used and/or exported by the yacc-generated parser.
 */

#ifndef _H_parser
#define _H_parser

  // here we need to include things needed for the yylval union
  // (types, classes, constants, etc.)
  
#include "scanner.hpp"            // for MaxIdentLen
#include "ast.hpp"		// in the union, we need their declarations
#include "ast_type.hpp"
#include "ast_decl.hpp"
#include "ast_expr.hpp"
#include "ast_stmt.hpp"

 
// Next, we want to get the exported defines for the token codes and
// typedef for YYSTYPE and exported global variable yylval.  These
// definitions are generated and written to the y.tab.h header file. But
// because that header does not have any protection against being
// re-included and those definitions are also present in the y.tab.c,
// we can get into trouble if we don't take precaution to not include if
// we are compiling y.tab.c, which we use the YYBISON symbol for. 
// Managing C headers can be such a mess! 

#ifndef YYBISON                 
#include "y.tab.hpp"              
#endif

int yyparse();              // Defined in the generated y.tab.c file
void InitParser();          // Defined in parser.y

#endif
