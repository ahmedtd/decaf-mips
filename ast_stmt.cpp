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

bool Program::scope_check(const map<const string, const Decl*> &current_scope)
{
    // Make a copy of the scope
    map<const string, const Decl*> introduced_scope;
    
    // Insert declarations introduced at program scope
    for(auto declp : *decls)
    {
        // cout << declp << " " << declp->id << " " << declp->id->name << endl;
        // cout << declp->location << endl;

        auto insert_info = introduced_scope.insert(
            make_pair(declp->id->name, declp)
        );
        
        bool insertion_succeeded = insert_info.second;
        auto insertion_it = insert_info.first;

        // Check if there was already an equivalent identifier at scope
        if(insertion_succeeded == false)
        {
            // Throw an appropriate error
            ReportError::DeclConflict(*declp, *((*insertion_it).second));
        }
    }

    // Shadow exterior scope with introduced scope
    introduced_scope.insert(begin(current_scope), end(current_scope));

    // Check children
    for(auto declp : *decls)
    {
        declp->scope_check(introduced_scope);
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


