/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */

#include <cassert>

#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
        
         
Decl::Decl(Identifier *n)
    : Node(*n->GetLocation()),
      id(n)
{
    assert(n != NULL);
    id->SetParent(this); 
}


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n)
{
    assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}
  

ClassDecl::ClassDecl(
    Identifier *n,
    NamedType *ex,
    vector<NamedType*> *imp,
    vector<Decl*> *m
) 
    : Decl(n),
      members(m),
      extends(ex),
      implements(imp)
{
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    assert(n != NULL && imp != NULL && m != NULL);     
    
    if(extends)
    {
        extends->SetParent(this);
    }

    for(auto implp : (*implements))
    {
        implp->SetParent(this);
    }

    for(auto memp : (*members))
    {
        memp->SetParent(this);
    }
}


InterfaceDecl::InterfaceDecl(Identifier *n, vector<Decl*> *m)
    : Decl(n),
      members(m)
{
    assert(n != NULL && m != NULL);

    for(auto memp : *members)
    {
        memp->SetParent(this);
    }
}

	
FnDecl::FnDecl(Identifier *n, Type *r, vector<VarDecl*> *d)
    : Decl(n),
      formals(d),
      returnType(r),
      body(NULL)
{
    assert(n != NULL && r!= NULL && d != NULL);
    
    returnType->SetParent(this);

    for(auto formp : *formals)
    {
        formp->SetParent(this);
    }
}

void FnDecl::SetFunctionBody(Stmt *b)
{ 
    body->SetParent(this);
}
