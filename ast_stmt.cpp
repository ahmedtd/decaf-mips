/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */

#include <cassert>
#include <map>
using std::make_pair;
#include <iostream>
using std::cout;
using std::endl;

#include "ast_stmt.hpp"
#include "ast_type.hpp"
#include "ast_decl.hpp"
#include "ast_expr.hpp"

#include "errors.hpp"

Program::Program(vector<Decl*> *d)
    : decls(d)
{
    assert(d != NULL);
    
    // for(auto declp : *decls)
    // {
    //     declp->parent(this);
    // }
}

bool Program::scope_check(const scope &exterior_scope) const
{
    scope local_scope(
        begin(*decls),
        end(*decls),
        ReportError::DeclConflict,
        exterior_scope);

    // Check all declarations at local scope
    for(auto declp : *decls)
    {
        declp->scope_check(local_scope);
    }

    return true;
}

StmtBlock::StmtBlock(vector<VarDecl*> *d, vector<Stmt*> *s)
    : decls(d),
      stmts(s)
{
    assert(d != NULL && s != NULL);

    // for(auto declp : *decls)
    // {
    //     declp->parent(this);
    // }

    // for(auto stmtp : *stmts)
    // {
    //     stmtp->parent(this);
    // }
}

bool StmtBlock::scope_check(const scope &exterior_scope) const
{
    // auto wrapper = [](const Decl &newer, const Decl &older) {
    //     cout << newer.id->name << endl;
    //     ReportError::DeclConflict(newer, older);
    // };

    // Build the local scope
    scope block_scope(
        begin(*decls),
        end(*decls),
        ReportError::DeclConflict,
        exterior_scope);

    // Each decl needs to check if its type is valid
    for(auto declp : *decls)
    {
        declp->scope_check(block_scope);
    }

    return true;
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b)
    : test(t),
      body(b)
{ 
    assert(t != NULL && b != NULL);
    // (test=t)->parent(this); 
    // (body=b)->parent(this);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b)
    : LoopStmt(t, b),
      init(i),
      step(s)                                        
{ 
    assert(i != NULL && t != NULL && s != NULL && b != NULL);
    // (init=i)->parent(this);
    // (step=s)->parent(this);
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb)
    : ConditionalStmt(t, tb),
      elseBody(eb)
{ 
    assert(t != NULL && tb != NULL); // else can be NULL
    // elseBody = eb;
    // if (elseBody) elseBody->parent(this);
}


ReturnStmt::ReturnStmt(yyltype loc, Expr *e)
    : Stmt(loc),
      expr(e)
{ 
    assert(e != NULL);
    // (expr=e)->parent(this);
}
  
PrintStmt::PrintStmt(vector<Expr*> *a)
    : args(a)
{    
    assert(a != NULL);

    // for(auto ap : *args)
    // {
    //     ap->parent(this);
    // }
}


