/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 *
 * pp5: You will need to extend the Stmt classes to implement
 * code generation for statements.
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include <deque>
using std::deque;
#include <memory>
using std::unique_ptr;
#include <vector>
using std::vector;

#include "ast.hh"
#include "codegen.hh"
#include "scope.hh"

class Decl;
class VarDecl;
class Expr;

class Program : public Node
{
protected:
    vector<Decl*> *decls;
     
public:
    Program(vector<Decl*> *declList);
    
    virtual void emit() const;
};

class Stmt : public Node
{
public:
    Stmt() : Node() {}
    Stmt(yyltype loc) : Node(loc) {}

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const = 0;
};

class StmtBlock : public Stmt 
{
protected:
    vector<VarDecl*> decls;
    vector<Stmt*> stmts;
    
public:
    StmtBlock(vector<VarDecl*> *variableDeclarations,
              vector<Stmt*> *statements);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;
};

  
// class ConditionalStmt : public Stmt
// {
// protected:
//     Expr *test;
//     Stmt *body;
  
// public:
//     ConditionalStmt(Expr *testExpr, Stmt *body);

//     virtual void emit(
//         CodeGenerator &generator,
//         deque<scope> &scope_chain,
//         size_t &num_local_bytes
//     ) const = 0;
// };

// class LoopStmt : public ConditionalStmt 
// {
// public:
//     LoopStmt(Expr *testExpr, Stmt *body)
//         : ConditionalStmt(testExpr, body) {}

//     virtual void emit(
//         CodeGenerator &generator,
//         deque<scope> &scope_chain,
//         size_t &num_local_bytes
//     ) const = 0;
// };

class ForStmt : public Stmt
{
  
public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;

protected:
    unique_ptr<Expr> m_init;
    unique_ptr<Expr> m_test;
    unique_ptr<Expr> m_step;
    
    unique_ptr<Stmt> m_body;
};

// class WhileStmt : public LoopStmt 
// {
// public:
//     WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
// };

class WhileStmt
    : public Stmt
{
public:
    WhileStmt(Expr *test,
              Stmt *body
    );

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;

protected:
    unique_ptr<Expr> m_test;
    unique_ptr<Stmt> m_body;
};

class IfStmt : public Stmt
{
public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;

protected:
    unique_ptr<Expr> m_test;
    unique_ptr<Stmt> m_then_body;
    unique_ptr<Stmt> m_else_body;
};

class BreakStmt : public Stmt 
{
public:
    BreakStmt(const yyltype &loc);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;
};

class ReturnStmt
    : public Stmt  
{
public:
    ReturnStmt(const yyltype &loc, Expr *expr);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;

protected:
    unique_ptr<Expr> m_expr;
};

class PrintStmt : public Stmt
{
public:
    PrintStmt(vector<Expr*> *arguments);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location 
    ) const;

protected:
    vector<Expr*> m_args;
    
};


#endif
