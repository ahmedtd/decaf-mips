/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */

#include <cassert>

#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"


Program::Program(vector<Decl*> *d)
    : decls(d)
{
    assert(d != NULL);
    
    for(auto declp : *decls)
    {
        declp->SetParent(this);
    }
}

void Program::Check()
{
    /* pp3: here is where the semantic analyzer is kicked off.
     *      The general idea is perform a tree traversal of the
     *      entire program, examining all constructs for compliance
     *      with the semantic rules.  Each node can have its own way of
     *      checking itself, which makes for a great use of inheritance
     *      and polymorphism in the node classes.
     */
}

StmtBlock::StmtBlock(vector<VarDecl*> *d, vector<Stmt*> *s)
    : decls(d),
      stmts(s)
{
    assert(d != NULL && s != NULL);

    for(auto declp : *decls)
    {
        declp->SetParent(this);
    }

    for(auto stmtp : *stmts)
    {
        stmtp->SetParent(this);
    }
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}


ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    assert(e != NULL);
    (expr=e)->SetParent(this);
}
  
PrintStmt::PrintStmt(vector<Expr*> *a)
    : args(a)
{    
    assert(a != NULL);

    for(auto ap : *args)
    {
        ap->SetParent(this);
    }
}


