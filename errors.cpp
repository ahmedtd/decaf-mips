/* File: errors.cc
 * ---------------
 * Implementation for error-reporting class.
 */


#include <cstdarg>
#include <cstdio>

#include <cassert>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;


#include "errors.hpp"
#include "scanner.hpp" // for GetLineNumbered
#include "ast_type.hpp"
#include "ast_expr.hpp"
#include "ast_stmt.hpp"
#include "ast_decl.hpp"

int ReportError::numErrors = 0;

void ReportError::UnderlineErrorInLine(const string &line, const yyltype &pos)
{
    cerr << line << endl;
    for (int i = 1; i <= pos.last_column; i++)
        cerr << (i >= pos.first_column ? '^' : ' ');
    cerr << endl;
}

 
 
void ReportError::OutputError(const yyltype &loc, const string &msg)
{
    numErrors++;
    fflush(stdout); // make sure any buffered text has been output
    // if (loc) {
    //     cerr << endl << "*** Error line " << loc->first_line << "." << endl;
    //     UnderlineErrorInLine(GetLineNumbered(loc->first_line), loc);
    // } else
    //     cerr << endl << "*** Error." << endl;

    // Because loc is no longer a pointer, always use the line number form
    cerr << endl << "*** Error line " << loc.first_line << "." << endl;
    UnderlineErrorInLine(GetLineNumbered(loc.first_line), loc);

    cerr << "*** " << msg << endl << endl;
}

void ReportError::OutputError(const string & msg)
{
    numErrors++;
    fflush(stdout);

    cerr << endl << "*** Error." << endl;
    cerr << "*** " << msg << endl << endl;
}

void ReportError::Formatted(const yyltype &loc, const char *format, ...) {
    va_list args;
    char errbuf[2048];
    
    va_start(args, format);
    vsprintf(errbuf,format, args);
    va_end(args);
    OutputError(loc, errbuf);
}

void ReportError::UntermComment() {
    OutputError("Input ends with unterminated comment");
}

void ReportError::InvalidDirective(int linenum) {
    yyltype ll = {0, linenum, 0, 0};
    OutputError(ll, "Invalid # directive");
}

void ReportError::LongIdentifier(const yyltype &loc, const char *ident) {
    stringstream s;
    s << "Identifier too long: \"" << ident << "\"";
    OutputError(loc, s.str());
}

void ReportError::UntermString(const yyltype &loc, const char *str) {
    stringstream s;
    s << "Unterminated string constant: " << str;
    OutputError(loc, s.str());
}

void ReportError::UnrecogChar(const yyltype &loc, char ch) {
    stringstream s;
    s << "Unrecognized char: '" << ch << "'" ;
    OutputError(loc, s.str());
}

void ReportError::DeclConflict(const Decl &decl, const Decl &prevDecl) {
    stringstream s;
    s << "Declaration of '" << decl << "' here conflicts with declaration on line " 
      << prevDecl.location.first_line;
    OutputError(decl.location, s.str());
}
  
void ReportError::OverrideMismatch(const Decl *fnDecl) {
    stringstream s;
    s << "Method '" << fnDecl << "' must match inherited type signature";
    OutputError(fnDecl->location, s.str());
}

void ReportError::InterfaceNotImplemented(const Decl *cd, const Type *interfaceType) {
    stringstream s;
    s << "Class '" << cd << "' does not implement entire interface '" << interfaceType << "'";
    OutputError(interfaceType->location, s.str());
}

void ReportError::IdentifierNotDeclared(const Identifier &ident, reasonT whyNeeded) {
    stringstream s;
    static const char *names[] =  {"type", "class", "interface", "variable", "function"};
    assert(whyNeeded >= 0 && whyNeeded <= sizeof(names)/sizeof(names[0]));
    s << "No declaration found for "<< names[whyNeeded] << " '" << ident << "'";
    OutputError(ident.location, s.str());
}

void ReportError::IncompatibleOperands(
    const Operator &op,
    const Type &lhs,
    const Type &rhs)
{
    stringstream s;
    s << "Incompatible operands: " << lhs << " " << op << " " << rhs;
    OutputError(op.location, s.str());
}
     
void ReportError::IncompatibleOperand(const Operator &op, const Type &rhs) {
    stringstream s;
    s << "Incompatible operand: " << op << " " << rhs;
    OutputError(op.location, s.str());
}

void ReportError::ThisOutsideClassScope(This *th) {
    OutputError(th->location, "'this' is only valid within class scope");
}

void ReportError::BracketsOnNonArray(Expr *baseExpr) {
    OutputError(baseExpr->location, "[] can only be applied to arrays");
}

void ReportError::SubscriptNotInteger(Expr *subscriptExpr) {
    OutputError(subscriptExpr->location, "Array subscript must be an integer");
}

void ReportError::NewArraySizeNotInteger(Expr *sizeExpr) {
    OutputError(sizeExpr->location, "Size for NewArray must be an integer");
}

void ReportError::NumArgsMismatch(Identifier *fnIdent, int numExpected, int numGiven) {
    stringstream s;
    s << "Function '"<< fnIdent << "' expects " << numExpected << " argument" << (numExpected==1?"":"s") 
      << " but " << numGiven << " given";
    OutputError(fnIdent->location, s.str());
}

void ReportError::ArgMismatch(Expr *arg, int argIndex, Type *given, Type *expected) {
  stringstream s;
  s << "Incompatible argument " << argIndex << ": " << given << " given, " << expected << " expected";
  OutputError(arg->location, s.str());
}

void ReportError::ReturnMismatch(ReturnStmt *rStmt, Type *given, Type *expected) {
    stringstream s;
    s << "Incompatible return: " << given << " given, " << expected << " expected";
    OutputError(rStmt->location, s.str());
}

void ReportError::FieldNotFoundInBase(Identifier *field, Type *base) {
    stringstream s;
    s << base << " has no such field '" << field <<"'";
    OutputError(field->location, s.str());
}
     
void ReportError::InaccessibleField(Identifier *field, Type *base) {
    stringstream s;
    s  << base << " field '" << field << "' only accessible within class scope";
    OutputError(field->location, s.str());
}

void ReportError::PrintArgMismatch(Expr *arg, int argIndex, Type *given) {
    stringstream s;
    s << "Incompatible argument " << argIndex << ": " << given
        << " given, int/bool/string expected";
    OutputError(arg->location, s.str());
}

void ReportError::TestNotBoolean(Expr *expr) {
    OutputError(expr->location, "Test expression must have boolean type");
}

void ReportError::BreakOutsideLoop(BreakStmt *bStmt) {
    OutputError(bStmt->location, "break is only allowed inside a loop");
}
  
/* Function: yyerror()
 * -------------------
 * Standard error-reporting function expected by yacc. Our version merely
 * just calls into the error reporter above, passing the location of
 * the last token read. If you want to suppress the ordinary "parse error"
 * message from yacc, you can implement yyerror to do nothing and
 * then call ReportError::Formatted yourself with a more descriptive 
 * message.
 */
void yyerror(const char *msg) {
    ReportError::Formatted(yylloc, "%s", msg);
}
