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

Stmt::Stmt()
{
}

Stmt::Stmt(const yyltype &loc)
    : Node(loc)
{
}

bool Stmt::scope_check(const scope &exterior_scope) const
{
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

    // Each statement also needs to check itself
    for(const Node *stmtp : *stmts)
    {
        stmtp->scope_check(block_scope);
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

bool ConditionalStmt::scope_check(const scope &exterior_scope) const
{
    return body->scope_check(exterior_scope);
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

bool ForStmt::scope_check(const scope &exterior_scope) const
{
    init->scope_check(exterior_scope);
    step->scope_check(exterior_scope);
    test->scope_check(exterior_scope);
    body->scope_check(exterior_scope);

    return true;
}

bool WhileStmt::scope_check(const scope &exterior_scope) const
{
    test->scope_check(exterior_scope);
    body->scope_check(exterior_scope);

    return true;
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb)
    : ConditionalStmt(t, tb),
      elseBody(eb)
{ 
    assert(t != NULL && tb != NULL); // else can be NULL
    // elseBody = eb;
    // if (elseBody) elseBody->parent(this);
}

bool IfStmt::scope_check(const scope &exterior_scope) const
{
    body->scope_check(exterior_scope);
    if(elseBody)
        elseBody->scope_check(exterior_scope);
    return true;
}

bool BreakStmt::scope_check(const scope &exterior_scope) const
{
    return true;
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e)
    : Stmt(loc),
      expr(e)
{ 
    assert(e != NULL);
    // (expr=e)->parent(this);
}

bool ReturnStmt::scope_check(const scope &exterior_scope) const
{
    return expr->scope_check(exterior_scope);
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

bool PrintStmt::scope_check(const scope &exterior_scope) const
{
    for(auto pexpr : *args)
    {
        pexpr->scope_check(exterior_scope);
    }

    return true;
}


