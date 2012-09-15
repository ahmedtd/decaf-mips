
%{

/* Code at top of file */

#include <map>
using std::map;
using std::pair;
using std::make_pair;
#include <string>
using std::string;

#include "errors.h"
%}

%x string_process

%x c_comment

%%

    /* Code local to the scanning routine */
%{
    int line_num = 1;
    std::map<std::string, std::string> replace_map;
%}

    /* Universal rule to increment line count when a newline is encountered */
\n ECHO; ++line_num;

    /* *********************** */
    /* Process string literals */
    /* *********************** */
\"                 BEGIN(string_process); ECHO;
<string_process>\n BEGIN(INITIAL); ECHO; ++line_num;
<string_process>\" BEGIN(INITIAL); ECHO;

    /* Process define macros */
#define" "[[:upper:]]+" "[^\n]* {
    string matched(yytext);
    
    auto index1 = matched.find(" ");
    auto index2 = matched.find(" ", index1 + 1);
    
    string symbol = matched.substr(index1 + 1, index2 - (index1+1));
    string replacement = matched.substr(index2 + 1);

    // printf("%s\n", matched.c_str());
    // printf("%lu %s\n", index1, symbol.c_str());
    // printf("%lu %s\n", index2, replacement.c_str());

    for(auto check_char : symbol)
    {
        if(! isupper(check_char))
            ReportError::InvalidDirective(line_num);
    }

    replace_map.insert(make_pair(symbol, replacement));
}

    /* Dummy target to match a malformed define expression */
#define[[:space:]]+[^\n]* ReportError::InvalidDirective(line_num);

    /* Process macro expansion*/
#[[:upper:]]+ {
    auto replace_it = replace_map.find(yytext+1);
    if(replace_it == replace_map.end())
        ReportError::InvalidDirective(line_num);
    else
        printf("%s", (*replace_it).second.c_str());
}

    /* Match all other directives, which are invalid */
#[[:alpha:]]+ ReportError::InvalidDirective(line_num);

    /* Strip comments */
"//"[^\n]*
"/*"                    BEGIN(c_comment);
<c_comment>[^*\n]*
<c_comment>[*]+[^*/\n]*
<c_comment>\n           ECHO; ++line_num;
<c_comment>"*"+"/"      BEGIN(INITIAL);
%%
