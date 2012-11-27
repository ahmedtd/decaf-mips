/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp5: You will need to extend the Decl classes to implement 
 * code generation for declarations.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include <deque>
using std::deque;
#include <vector>
using std::vector;

#include "ast.hh"
#include "codegen.hh"
#include "scope.hh"

class Type;
class Identifier;
class Stmt;

class Decl : public Node 
{
protected:
    Identifier *id;
    string m_name;
  
public:
    Decl(Identifier *name);

    const string& name() const;

    virtual void emit(CodeGenerator &generator, deque<scope> &scope_chain) const = 0;

    friend ostream& operator<<(ostream& out, const Decl &to_print);
};

class VarDecl
    : public Decl 
{
protected:
    Type m_type;
    
public:
    VarDecl(Identifier *name, Type *type);

    const Type& type() const;
    
    virtual void emit(CodeGenerator &generator, deque<scope> &scope_chain) const;
};

class ClassDecl
    : public Decl 
{
protected:
    vector<FnDecl*> m_functions;
    vector<VarDecl*> m_vars;
    Type *extends;
    vector<Type*> *implements;

    scope local_scope;

public:
    ClassDecl(Identifier *name,
              Type *extends, 
              vector<Type*> *implements,
              vector<Decl*> *members);

    virtual void emit(CodeGenerator &generator, deque<scope> &scope_chain) const;

    const size_t mem_size() const;
};

class InterfaceDecl
    : public Decl 
{
protected:
    vector<Decl*> *members;
    
public:
    InterfaceDecl(Identifier *name, vector<Decl*> *members);

    virtual void emit(CodeGenerator &generator, deque<scope> &scope_chain) const;
};

class FnDecl
    : public Decl 
{
protected:
    vector<VarDecl*> *formals;
    Type m_returntype;;
    Stmt *body;
    
public:
    FnDecl(Identifier *name, Type *returnType, vector<VarDecl*> *formals);

    const Type& returntype() const;

    void SetFunctionBody(Stmt *b);

    void emit(CodeGenerator &generator,
              deque<scope> &scope_chain,
              const string &label_prefix = "") const;
};

#endif
