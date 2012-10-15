/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp3: You will need to extend the Decl classes to implement 
 * semantic processing including detection of declaration conflicts 
 * and managing scoping issues.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include <ostream>
using std::ostream;
#include <vector>
using std::vector;

#include "ast.hpp"

#include "scope.hpp"

class Type;
class NamedType;
class Identifier;
class StmtBlock;

class Decl : public Node 
{
public:
    Decl(Identifier *name);

    const Identifier& ident() const;
    
protected: 
    const Identifier *m_ident;
};

ostream& operator<<(std::ostream& out, const Decl &d);

class VarDecl : public Decl 
{
protected:
    Type *m_type;
    
public:
    VarDecl(Identifier *name, Type *type);

    const Type& type() const;

    virtual bool scope_check(const scope &exterior_scope) const;
};

class ClassDecl : public Decl 
{
protected:
    vector<Decl*> *members;
    Type *extends;
    vector<Type*> *implements;

public:
    ClassDecl(Identifier *name, Type *extends, 
              vector<Type*> *implements, vector<Decl*> *members);

    virtual bool scope_check(const scope  &exterior_scope) const;
};

class InterfaceDecl : public Decl 
{
protected:
    vector<Decl*> *members;
    
public:
    InterfaceDecl(Identifier *name, vector<Decl*> *members);

    virtual bool scope_check(const scope &exterior_scope) const;
};

class FnDecl : public Decl 
{
protected:
    vector<VarDecl*> *formals;
    Type *returnType;
    StmtBlock *body;
    
public:
    FnDecl(Identifier *name, Type *returnType, vector<VarDecl*> *formals);
    void SetFunctionBody(StmtBlock *b);

    virtual bool scope_check(const scope &exterior_scope) const;
};

#endif
