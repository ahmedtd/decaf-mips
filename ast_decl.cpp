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
      m_ident(n)
{
    assert(n != NULL);
    //id->parent(this); 
}

const Identifier& Decl::ident() const
{
    return *m_ident;
}

ostream& operator<<(std::ostream& out, const Decl &d)
{
    return out << d.ident();
}

VarDecl::VarDecl(Identifier *n, Type *t)
    : Decl(n),
      m_type(t)
{
    assert(n != NULL && t != NULL);
    //(type=t)->parent(this);
}

const Type& VarDecl::type() const
{
    return *m_type;
}

bool VarDecl::scope_check(const scope &exterior_scope) const
{
    auto type_reporter = [](const Type &given_type) {
        ReportError::IdentifierNotDeclared(
            given_type.ident(),
            reasonT::LookingForType
        );
    };

    // Check if the type of the var is present in the exterior scope
    vector<Type*> type_vec = {m_type};
    exterior_scope.types_exist(
        begin(type_vec),
        end(type_vec),
        type_reporter
    );
        
    return true;
}

ClassDecl::ClassDecl(
    Identifier *n,
    Type *ex,
    vector<Type*> *imp,
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

bool ClassDecl::scope_check(const scope &exterior_scope) const
{
    if(extends)
    {
        auto extend_reporter = [](const Type &given_type) {
            ReportError::IdentifierNotDeclared(
                given_type.ident(),
                reasonT::LookingForClass
            );
        };

        // First check that the type we extend exists
        vector<Type*> extend_types = {extends};
        exterior_scope.types_exist(
            begin(extend_types),
            end(extend_types),
            extend_reporter,
            scope::type_selector::class_only
        );
    }

    auto implement_reporter = [](const Type &given_type) {
        ReportError::IdentifierNotDeclared(
            given_type.ident(),
            reasonT::LookingForInterface
        );
    };

    // Check that any types we implement exist
    exterior_scope.types_exist(
        begin(*implements),
        end(*implements),
        implement_reporter,
        scope::type_selector::interface_only
    );
        
    // Check our inner scope
    scope bodyscope(begin(*members),
                    end(*members),
                    ReportError::DeclConflict,
                    exterior_scope);

    for(auto declp : *members)
    {
        declp->scope_check(bodyscope);
    }

    // Search for the declaration of the class we're extending
    
    // Search for the declaration of all the classes we're implementing

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

bool InterfaceDecl::scope_check(const scope &exterior_scope) const
{
    // An interface needs to check its body for consistency
    scope bodyscope(begin(*members),
                    end(*members),
                    ReportError::DeclConflict,
                    exterior_scope);

    for(auto declp : *members)
    {
        // FnDecls stil need to have their formal parameters scope checked.
        declp->scope_check(bodyscope);
    }
    
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

bool FnDecl::scope_check(const scope &exterior_scope) const
{
    // The FnDecl is responsible for checking its formal parameters
    scope paramscope(begin(*formals),
                     end(*formals),
                     ReportError::DeclConflict,
                     exterior_scope);

    // Ensure that the types of the formal parameters exist
    auto type_reporter = [](const Type &given_type) {
        ReportError::IdentifierNotDeclared(
            given_type.ident(),
            reasonT::LookingForType
        );
    };
    // Aggregate all of our relevant identifiers
    vector<const Type*> needed_types;
    for(auto vardeclp : *formals)
    {
        needed_types.push_back(&(vardeclp->type()));
    }
    paramscope.types_exist(
        begin(needed_types),
        end(needed_types),
        type_reporter
    );

    // The StmtBlock it embodies will check the body's scope
    if(body)
        body->scope_check(paramscope);

    return true;
}
