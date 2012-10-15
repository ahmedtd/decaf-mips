
#ifndef DECAF_SCOPE_HPP
#define DECAF_SCOPE_HPP

#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

class Decl;
class ClassDecl;
class InterfaceDecl;

class basic_scope
{
public:
    enum class type_selector
    {
        any,
        class_only,
        interface_only
    };
public:

    // Construct an empty scope
    basic_scope();

    // Construct a scope with Decls from the given range
    template<class LocalScopeIterator, class ReportingFunc>
    basic_scope(
        LocalScopeIterator begin_local_scope,
        LocalScopeIterator end_local_scope,
        ReportingFunc rep_func,
        const basic_scope &exterior_scope
    );

    // ReportingFunc takes a (const Type &) parameter
    template <class TypeIterator, class ReportingFunc>
    bool types_exist(
        TypeIterator begin_types,
        TypeIterator end_types,
        ReportingFunc rep_func,
        type_selector restriction = type_selector::any
    ) const;

protected:
    map<const string, const Decl*> m_decls_at_scope;
};

template<class LocalScopeIterator,
         class ReportingFunc>
basic_scope::basic_scope(
    LocalScopeIterator begin_local_scope,
    LocalScopeIterator end_local_scope,
    ReportingFunc rep_func,
    const basic_scope &exterior_scope
)
{
    // Insert declarations from the program scope, calling the reporting
    // function on any conflicts
    
    for(; begin_local_scope != end_local_scope; ++begin_local_scope)
    {
        auto declp = *begin_local_scope;

        auto insert_info = m_decls_at_scope.insert(
            make_pair(declp->ident().name, declp)
        );

        bool insertion_succeeded = insert_info.second;
        auto insertion_it = insert_info.first;

        if(insertion_succeeded == false)
        {
            rep_func(*declp, *((*insertion_it).second));
        }
    }

    // After reporting any errors in the local scope, fill in all unshadowed
    // names from the exterior scope
    m_decls_at_scope.insert(begin(exterior_scope.m_decls_at_scope),
                            end(exterior_scope.m_decls_at_scope));
}

template <class TypeIterator, class ReportingFunc>
bool basic_scope::types_exist(
    TypeIterator begin_types,
    TypeIterator end_types,
    ReportingFunc rep_func,
    type_selector restriction
) const
{
    // Build the set of possible types based on the type selector
    set<string> respondent_types;
    // Add all the classes at the current scope
    for(auto declpair : m_decls_at_scope)
    {
        auto declp = declpair.second;
        const string &decl_name = declpair.first;

        const ClassDecl *cdeclp = dynamic_cast<const ClassDecl*>(declp);
        const InterfaceDecl *ideclp = dynamic_cast<const InterfaceDecl*>(declp);

        if((restriction == type_selector::any
            || restriction == type_selector::class_only)
           && cdeclp)
        {
            respondent_types.insert(decl_name);
        }
        if((restriction == type_selector::any
            || restriction == type_selector::interface_only)
           && ideclp)
        {
            respondent_types.insert(decl_name);
        }
        if(restriction == type_selector::any)
        {
            set<string> basic_types = {"int",
                                   "double",
                                   "bool",
                                   "void",
                                   "null",
                                   "string",
                                   "error"};
            
            respondent_types.insert(begin(basic_types),
                                    end(basic_types));
        }
    }
    
    // Check if each type is in the respondent type vector
    for(; begin_types != end_types; ++begin_types)
    {
        const string &request_name = (*begin_types)->bare().ident().name;
        
        if(respondent_types.count(request_name) < 1)
        {
            rep_func((*begin_types)->bare());
        }
    }
    
    return true;
}

typedef basic_scope scope;

#endif
