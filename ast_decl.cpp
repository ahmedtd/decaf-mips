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

set<string> Decl::depends_on() const
{
    return {};
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

    //cout << *m_type << endl;

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
    vector<Decl*> *members
) 
    : Decl(n),
      //members(m),
      extends(ex),
      implements(imp)
{
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    assert(n != NULL && imp != NULL && members != NULL);     
    
    // We need to do this more elegantly
    for(auto declp : *members)
    {
        m_members.insert(make_pair(declp->ident().name, declp));
    }
}

set<string> ClassDecl::depends_on() const
{
    set<string> retval;
    
    if(extends)
    {
        retval.insert(extends->ident().name);
    }
    for(auto implp : *implements)
    {
        retval.insert(implp->ident().name);
    }

    return retval;
}

bool ClassDecl::scope_check(const scope &exterior_scope) const
{
    vector<const Decl*> members_declp;
    for(auto decl_pair : m_members)
    {
        members_declp.push_back(decl_pair.second);
    }

    // Check our inner scope
    scope bodyscope(begin(members_declp),
                    end(members_declp),
                    ReportError::DeclConflict,
                    exterior_scope);

    vector<Type*> extend_types;
    if(extends)
    {
        extend_types.push_back(extends);
    }
    auto extend_reporter = [](const Type &given_type) {
        ReportError::IdentifierNotDeclared(
            given_type.ident(),
            reasonT::LookingForClass
        );
    };

    // First check that the type we extend exists
    map<const string, const Decl*> extend_decls
        = exterior_scope.types_exist(
            begin(extend_types),
            end(extend_types),
            extend_reporter,
            scope::type_selector::class_only
        );

    auto implement_reporter = [](const Type &given_type) {
        ReportError::IdentifierNotDeclared(
            given_type.ident(),
            reasonT::LookingForInterface
        );
    };

    // Check that any types we implement exist
    map<const string, const Decl*> implement_decls
    = exterior_scope.types_exist(
        begin(*implements),
        end(*implements),
        implement_reporter,
        scope::type_selector::interface_only
    );

    for(auto declp : members_declp)
    {
        declp->scope_check(bodyscope);
    }

    // For each type we implement, ensure that we haven't mis-implemented any
    // functions
    for(auto imp_pair : implement_decls)
    {
        const string &imp_name = imp_pair.first;
        const InterfaceDecl *imp_decl
            = dynamic_cast<const InterfaceDecl*>(imp_pair.second);

        // For each of our members
        for(auto memberpair : m_members)
        {
            const string &member_name = memberpair.first;
            const Decl *member_declp = memberpair.second;
                
            // Does this member fit the definition of a member of the class
            // we're extending?
                
            auto override_it = imp_decl->m_members.find(member_name);
                
            // If we found something, we've got a conflicting name
            if(override_it != end(imp_decl->m_members))
            {
                // Now, the only way not to throw an error is if our
                // declaration is a function, the detected declaration is a
                // function, and they have the exact same signature.

                const FnDecl* ourfnp = dynamic_cast<const FnDecl*>(member_declp);
                const FnDecl* theirfnp
                    = dynamic_cast<const FnDecl*>((*override_it).second);

                if(!(ourfnp && theirfnp && signatures_equal(*ourfnp, *theirfnp)))
                {
                    ReportError::OverrideMismatch(*member_declp);
                }
            }
        }

        // For each of the members of the interface being implemented
        for(auto theirpair : imp_decl->m_members)
        {
            const string &their_name = theirpair.first;
            
            // Check to see if we have a member by that name
            if(m_members.count(their_name) < 1)
            {
                for(auto ptype : *implements)
                {
                    if(ptype->ident().name == imp_name)
                    {
                        ReportError::InterfaceNotImplemented(*this, *ptype);
                    }
                }
            }
        }
    }

    // For each type we extend (only one in vanilla Decaf), check that we extend
    // it properly
    for(auto extend_pair : extend_decls)
    {
        const string &extend_name = extend_pair.first;
        const ClassDecl *extend_decl
            = dynamic_cast<const ClassDecl*>(extend_pair.second);

        // For each of our members
        for(auto memberpair : m_members)
        {
            const string &member_name = memberpair.first;
            const Decl *member_declp = memberpair.second;
                
            // Does this member fit the definition of a member of the class
            // we're extending?
                
            auto override_it = extend_decl->m_members.find(member_name);
                
            // If we found something, we've got a conflicting name
            if(override_it != end(extend_decl->m_members))
            {
                // Now, the only way not to throw an error is if our
                // declaration is a function, the detected declaration is a
                // function, and they have the exact same signature.

                const FnDecl* ourfnp = dynamic_cast<const FnDecl*>(member_declp);
                const FnDecl* theirfnp
                    = dynamic_cast<const FnDecl*>((*override_it).second);

                if(ourfnp && theirfnp)
                {
                    if(! signatures_equal(*ourfnp, *theirfnp))
                    {
                        ReportError::OverrideMismatch(*member_declp);
                    }
                }
                else if(!ourfnp || !theirfnp)
                {
                    ReportError::DeclConflict(*member_declp, *((*override_it).second));
                }
            }
        }
    }

    return true;
}

InterfaceDecl::InterfaceDecl(Identifier *n, vector<Decl*> *members)
    : Decl(n)//,
      //members(m)
{
    assert(n != NULL && members != NULL);

    for(auto declp : *members)
    {
        m_members.insert(make_pair(declp->ident().name, declp));
    }
}

bool InterfaceDecl::scope_check(const scope &exterior_scope) const
{
    vector<const Decl*> members_declp;
    for(auto decl_pair : m_members)
    {
        members_declp.push_back(decl_pair.second);
    }

    // An interface needs to check its body for consistency
    scope bodyscope(begin(members_declp),
                    end(members_declp),
                    ReportError::DeclConflict,
                    exterior_scope);

    for(auto declp : members_declp)
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

bool signatures_equal(const FnDecl &lop, const FnDecl &rop)
{
    // Check if the return types are equal
    if(*(lop.returnType) == *(rop.returnType))
    {
        const vector<VarDecl*> &lop_formals = *(lop.formals);
        const vector<VarDecl*> &rop_formals = *(rop.formals);

        if((lop_formals.size() == rop_formals.size())
           && equal(begin(lop_formals),
                    end(lop_formals),
                    begin(rop_formals),
                    [](const VarDecl *ldecl, const VarDecl *rdecl) {
                        return *(ldecl->m_type) == *(rdecl->m_type);
                    }
           )
        )
        {
            return true;
        }
    }
    
    return false;
}

