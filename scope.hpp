
#ifndef DECAF_SCOPE_HPP
#define DECAF_SCOPE_HPP

#include <map>
using std::map;
#include <string>
using std::string;

class Decl;

class basic_scope
{
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

    template <class TypeIterator, class ReportingFunc>
    bool types_exist(
        TypeIterator begin_decls,
        TypeIterator end_decls,
        ReportingFunc rep_func
    );

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
            make_pair(declp->id->name, declp)
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

typedef basic_scope scope;

#endif
