/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */

#include <cassert>

#include <iostream>
using std::cout;
using std::endl;

#include "ast_decl.hpp"
#include "ast_type.hpp"
#include "ast_stmt.hpp"

#include "errors.hpp"
         
Decl::Decl(Identifier *n)
    : Node(n->location),
      id(n)
{
    assert(n != NULL);
    //id->parent(this); 
}

ostream& operator<<(std::ostream& out, const Decl &d)
{
    return out << *(d.id);
}

VarDecl::VarDecl(Identifier *n, Type *t)
    : Decl(n),
      type(t)
{
    assert(n != NULL && t != NULL);
    //(type=t)->parent(this);
}

bool VarDecl::scope_check(const scope &exterior_scope)
{
    // Does a vardecl have anything to do in terms of a scope check?
    return true;
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
    
    // if(extends)
    // {
    //     extends->parent(this);
    // }

    // for(auto implp : (*implements))
    // {
    //     implp->parent(this);
    // }

    // for(auto memp : (*members))
    // {
    //     memp->parent(this);
    // }
}

bool ClassDecl::scope_check(const scope &exterior_scope)
{
    return true;
}

InterfaceDecl::InterfaceDecl(Identifier *n, vector<Decl*> *m)
    : Decl(n),
      members(m)
{
    assert(n != NULL && m != NULL);

    // for(auto memp : *members)
    // {
    //     memp->parent(this);
    // }
}

bool InterfaceDecl::scope_check(const scope &exterior_scope)
{
    return true;
}
	
FnDecl::FnDecl(Identifier *n, Type *r, vector<VarDecl*> *d)
    : Decl(n),
      formals(d),
      returnType(r),
      body(NULL)
{
    assert(n != NULL && r!= NULL && d != NULL);
    
    // returnType->parent(this);

    // for(auto formp : *formals)
    // {
    //     formp->parent(this);
    // }
}

void FnDecl::SetFunctionBody(StmtBlock *b)
{
    body = b;
    // body->parent(this);
}

bool FnDecl::scope_check(const scope &exterior_scope)
{
    // The FnDecl is responsible for checking its formal parameters
    scope paramscope(begin(*formals),
                     end(*formals),
                     ReportError::DeclConflict,
                     exterior_scope);

    // Ensure that the types of the formal parameters exist
    auto identifier_reporter = [](const Identifier &ident) {
        ReportError::IdentifierNotDeclared(
            ident,
            ReportError::reasonT::LookingForType
        );
    }
    // Aggregate all of our relevant identifiers
    vector<Identifier*> needed_types;
    for(auto vardeclp : *formals)
    {
        needed_types.push_back(vardeclp->
    paramscope.types_exist(begin(*formals), end(*formals), identifier_reporter);

    // The StmtBlock it embodies will check the body's scope
    body->scope_check(paramscope);

    return true;
}
