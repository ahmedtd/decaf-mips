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

#include <vector>
using std::vector;

#include "ast.h"

class Type;
class NamedType;
class Identifier;
class Stmt;

class Decl : public Node 
{
  protected:
    Identifier *id;
  
  public:
    Decl(Identifier *name);
    friend std::ostream& operator<<(std::ostream& out, Decl *d) { return out << d->id; }
};

class VarDecl : public Decl 
{
  protected:
    Type *type;
    
  public:
    VarDecl(Identifier *name, Type *type);
};

class ClassDecl : public Decl 
{
  protected:
    vector<Decl*> *members;
    NamedType *extends;
    vector<NamedType*> *implements;

  public:
    ClassDecl(Identifier *name, NamedType *extends, 
              vector<NamedType*> *implements, vector<Decl*> *members);
};

class InterfaceDecl : public Decl 
{
  protected:
    vector<Decl*> *members;
    
  public:
    InterfaceDecl(Identifier *name, vector<Decl*> *members);
};

class FnDecl : public Decl 
{
  protected:
    vector<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    
  public:
    FnDecl(Identifier *name, Type *returnType, vector<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
};

#endif
