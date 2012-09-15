/* File: dppmain.cc
 * ----------------
 * This file defines the main() routine for the preprocessor, 
 * the filtering tool which runs before the compiler.
 */

#include "scanner.h"

#include "errors.h"

#include <iostream>
#include <map>
#include <string>

#include <tuple>
using std::get;

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::getline;

using std::istream;
using std::ostream;
using std::skipws;

using std::map;

// // String literals are treated as more than simple passthroughs because macro
// // substitution is not supposed to occur inside them.
// void process_string_literal(istream &in, ostream &out, int &line_number)
// {
//     typedef istream::char_type   char_type;
//     typedef istream::int_type    int_type;
//     typedef istream::traits_type traits_type;

//     int quote_count = 0;

//     char next_char;
//     while(in >> next_char)
//     {
//         out << next_char;

//         if(next_char == '\"')
//         {
//             quote_count++;

//             if(quote_count == 2)
//                 return;
//         }
//         else if(next_char == '\n')
//         {
//             line_number++;
//             return;
//         }
//     }
// }

// void process_pragma(istream &in,
//                     ostream &out,
//                     int &line_number,
//                     map<string, string> &define_dict)
// {
//     // Pull off the octothorpe
//     in.get();

//     // Consume the pragma token -- consume until the current character is
//     // nonalphabetic
//     pragma_tok = match_define_name

//     if(pragma_tok == "define")
//     {
//         // Pull out the new symbol to define
//         string define_newsym;
//         in >> skipws >> define_newsym;

//         // Check that it's all upper-case
//         for(auto checkchar : define_newsym)
//         {
//             if(! isupper(checkchar))
//             {
//                 ReportError::InvalidDirective(line_number);
//                 return;
//             }
//         }
        
//         // Pull out the space before the replacement
//         in.get();

//         // Pull out the replacement
//         string define_replacement;
//         getline(in, define_replacement);
        
//         // The ending newline was consumed from the string, so we need to act as
//         // if one were read
//         out << '\n';
//         line_number++;

//         // Enter the new definition into the replacement dictionary
//         define_dict[define_newsym] = define_replacement;

//         return;
//     }
    
//     for(auto checkchar : pragma_tok)
//     {
//         if(! isupper(checkchar))
//         {
//             cout << "not all upper:" << checkchar << endl;
//             ReportError::InvalidDirective(line_number);
//             return;
//         }
//     }

//     auto replace_it = define_dict.find(pragma_tok);
//     if(replace_it == define_dict.end())
//     {
//         cout << "definition not found: " << pragma_tok << endl;
//         ReportError::InvalidDirective(line_number);
//         return;
//     }
//     else
//     {
//         out << get<1>(*replace_it);
//     }
// }

// void process_comment(istream &in, ostream &out, int &line_number)
// {
//     // Consume the beginning forward slash
//     char first_slash;
//     cin >> first_slash;

//     // Get the next character to determine if this is a multiline comment or a
//     // single-line comment (or no comment)
//     char next_char;
//     in >> next_char;
//     if(! in)
//     {
//         out << '/';
//         return;
//     }

//     if(next_char == '/')
//     {
//         // Kill to end of line
//         string line;
//         getline(in, line);
        
//         // Emit a newline
//         out << '\n';
//     }
//     else if(next_char == '*')
//     {
//         // move in a loop, character by character
//         char cur_char;
//         bool last_was_asterisk = false;
//         while(in >> cur_char)
//         {
//             if(cur_char == '\n')
//             {
//                 last_was_asterisk = false;
//                 out << '\n';
//             }
//             else if(cur_char == '*')
//             {
//                 last_was_asterisk = true;
//             }
//             else if(cur_char == '/')
//             {
//                 if(last_was_asterisk)
//                     return;
//                 last_was_asterisk = false;
//             }
//             else
//             {
//                 last_was_asterisk = false;
//             }
//         }
        
//         ReportError::UntermComment();
//     }
//     else
//     {
//         out << '/';
//     }

//     return;
// }

int main(int argc, char *argv[])
{
    // map<string, string> define_map;

    // int line_number = 0;

    // char next_char;
    // while(cin >> next_char)
    // {
    //     if(next_char == '/')
    //     {
    //         cin.putback(next_char);
    //         process_comment(cin, cout, line_number);
    //     }
    //     else if(next_char == '#')
    //     {
    //         cin.putback(next_char);
    //         process_pragma(cin, cout, line_number, define_map);
    //     }
    //     else if(next_char == '\"')
    //     {
    //         cin.putback(next_char);
    //         process_string_literal(cin, cout, line_number);
    //     }
    //     else if(next_char == '\n')
    //     {
    //         line_number++;
    //         cout << next_char;
    //     }
    //     else
    //     {
    //         cout << next_char;
    //     }
    // }

    while(yylex() != 0);
}
