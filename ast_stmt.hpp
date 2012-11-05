/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 *
 * pp3: You will need to extend the Stmt classes to implement
 * semantic analysis for rules pertaining to statements.
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include <vector>
using std::vector;

#include "ast.hpp"

class Decl;
class VarDecl;
class Expr;

// Program has a scope that represents 
class Program : public Node
{
protected:
    vector<Decl*> *decls;
    
public:
    Program(vector<Decl*> *declList);

    virtual bool scope_check(
        const scope &exterior_scope = (scope())
    ) const;
};

class Stmt : public Node
{
public:
    Stmt();
    Stmt(const yyltype &loc);

    virtual bool scope_check(const scope &exterior_scope) const;
};

class StmtBlock : public Stmt 
{
public:
    StmtBlock(vector<VarDecl*> *variableDeclarations, vector<Stmt*> *statements);

    virtual bool scope_check(const scope &exterior_scope) const;
protected:
    vector<VarDecl*> *decls;
    vector<Stmt*> *stmts;
};

  
class ConditionalStmt : public Stmt
{
protected:
    Expr *test;
    Stmt *body;
  
public:
    ConditionalStmt(Expr *testExpr, Stmt *body);

    virtual bool scope_check(const scope &exterior_scope) const;
};

class LoopStmt : public ConditionalStmt 
{
public:
    LoopStmt(Expr *testExpr, Stmt *body)
        : ConditionalStmt(testExpr, body) {}
};

class ForStmt : public LoopStmt 
{
protected:
    Expr *init, *step;
  
public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);

    virtual bool scope_check(const scope &exterior_scope) const;
};

class WhileStmt : public LoopStmt 
{
public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}

    virtual bool scope_check(const scope &exterior_scope) const;
};

class IfStmt : public ConditionalStmt 
{
protected:
    Stmt *elseBody;

    virtual bool scope_check(const scope &exterior_scope) const;
  
public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
};

class BreakStmt : public Stmt 
{
public:
    BreakStmt(yyltype loc) : Stmt(loc) {}

    virtual bool scope_check(const scope &exterior_scope) const;
};

class ReturnStmt : public Stmt  
{
protected:
    Expr *expr;
  
public:
    ReturnStmt(yyltype loc, Expr *expr);

    virtual bool scope_check(const scope &exterior_scope) const;
};

class PrintStmt : public Stmt
{
protected:
    vector<Expr*> *args;
    
public:
    PrintStmt(vector<Expr*> *arguments);

    virtual bool scope_check(const scope &exterior_scope) const;
};


#endif
