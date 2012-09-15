/* File:  scanner.l
 * ----------------
 * Lex input file to generate the scanner for the compiler.
 */

%{

#include <ctime>

#include <iostream>
using std::cout;
using std::flush;
using std::endl;

#include <cstdio>

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <string.h>
#include "scanner.h"
#include "utility.h" // for PrintDebug()
#include "errors.h"

YYSTYPE yylval;  // manually declared for pp1, later Yacc provides

struct yyltype yylloc; // manually declared for pp1, later Yacc provides

static void DoBeforeEachAction(); 
#define YY_USER_ACTION DoBeforeEachAction();

%}

%x STRING_CONTENT
%x STRING_TERMINATE

%%
    /* ***************** */
    /* Begin match rules */
    /* ***************** */

    /* Match whitespace and do nothing */
[[:space:]]+

    /* ********************* */
    /* Match string literals */
    /* ********************* */

\"[^\n\"]*\" {
    auto literal_length = strlen(yytext);
    char *matched = new char[literal_length + 1];
    strncpy(matched, yytext, literal_length + 1);

    yylval.stringConstant = matched;
    return T_StringConstant;
}

\"[^\"]\n {
    ReportError::UntermString(&yylloc, yytext);
}

    /* ************************ */
    /* Handle boolean constants */
    /* ************************ */

true {
    yylval.boolConstant = true;
    return T_BoolConstant;
}

false {
    yylval.boolConstant = false;
    return T_BoolConstant;
}

    /* *********************** */
    /* Handle double constants */
    /* *********************** */

[0-9]+"."[0-9]*((e|E)("+"|"-")?[0-9]+)? {
    yylval.doubleConstant = lexical_cast<double>(yytext);
    return T_DoubleConstant;
}

    /* ************************ */
    /* Handle integer constants */
    /* ************************ */

0 {
    yylval.integerConstant = lexical_cast<int>(yytext);
    return T_IntConstant;
}
    
[1-9][0-9]* {
    yylval.integerConstant = lexical_cast<int>(yytext);
    return T_IntConstant;
}

0[0-7]+ {
    yylval.integerConstant = lexical_cast<int>(yytext);
    return T_IntConstant;
}

0x[0-9a-fA-F]+ {
    yylval.integerConstant = lexical_cast<int>(yytext);
    return T_IntConstant;
}

    /* ***************** */
    /* Match fixed types */
    /* ***************** */

void {
    return T_Void;
}

int {
    return T_Int;
}

double {
    return T_Double;
}

bool {
    return T_Bool;
}

class {
    return T_Class;
}

string {
    return T_String;
}

    /* *************** */
    /* Match operators */
    /* *************** */
"<=" return T_LessEqual;

">=" return T_GreaterEqual;

"==" return T_Equal;

"!=" return T_NotEqual;

"&&" return T_And;

"||" return T_Or;

"<" return '<';

">" return '>';

"+" return '+';

"-" return '-';

"*" return '*';

"/" return '/';

"%" return '%';

"." return '.';

"(" return '(';

")" return ')';

"=" return '=';

"!" return '!';

"," return ',';

"[" return '[';

"]" return ']';

"{" return '{';

"}" return '}';

    /* ********************* */
    /* Match other furniture */
    /* ********************* */

";" return ';';

"break"  return T_Break;

"return" return T_Return;

"for" return T_For;

"if" return T_If;

"else" return T_Else;

"extends" return T_Extends;

"this" return T_This;

"implements" return T_Implements;

"interface" return T_Interface;

"null" return T_Null;

 /* "ReadInteger" return T_ReadInteger; */

 /* "ReadLine" return T_ReadLine; */

"New" return T_New;

"NewArray" return T_NewArray;

    /* ***************** */
    /* Match identifiers */
    /* ***************** */

[[:alpha:]][[:alnum:]_]{0,30} {
    strncpy(yylval.identifier, yytext, 32);

    return T_Identifier;
}

[[:alpha:]][[:alnum:]_]{31,} {
    ReportError::LongIdentifier(&yylloc, yytext);
}

%%
    /* *************** */
    /* Begin user code */
    /* *************** */

void InitScanner()
{
    PrintDebug("lex", "Initializing scanner");
    yy_flex_debug = false;

    // Initialize yyloc
    yylloc.timestamp = std::time(nullptr);
    yylloc.first_line = 1;
    yylloc.first_column = 0;
    yylloc.last_line = 1;
    yylloc.last_column = 0;
    yylloc.text = nullptr;
}


/* Function: DoBeforeEachAction()
 * ------------------------------
 * This function is installed as the YY_USER_ACTION. This is a place
 * to group code common to all actions.
 */
static void DoBeforeEachAction()
{
    string matched(yytext);

    yylloc.timestamp = std::time(nullptr);
    
    // The end of the last token matched is the beginning of this token,
    // assuming that no characters are escaping us.
    yylloc.first_line   = yylloc.last_line;
    yylloc.first_column = yylloc.last_column + 1;

    // Count the number of newlines in the string
    // There must be a more elegant way to do this
    for(auto check_char : matched)
    {
        if(check_char == '\n')
        {
            yylloc.last_line++;
            yylloc.last_column = 0;
        }
        else
        {
            yylloc.last_column++;
        }
    }
}
