
#ifndef DECAF_SCOPE_HPP
#define DECAF_SCOPE_HPP

template<class Container>
class basic_scope
{
public:

    // Construct an empty scope
    basic_scope();

    // Construct a scope with Decls from the given range
    template<class LocalScopeIterator, class ReportingFunc>
    basic_scope(
        InputIterator begin_local_scope,
        InputIterator end_local_scope,
        ReportingFunc rep_func
    );

protected:
    Container<const string, const Decl*> m_decls_at_scope;
};

template<class Container>
basic_scope<Container>::basic_scope()
{
    
}

template<class Container,class LocalScopeIterator, class ReportingFunc>
basic_scope<Container>::basic_scope(
    InputIterator begin_local_scope,
    InputIterator end_local_scope,
    ReportingFunc rep_func,
    basic_scope<Container> exterior_scope
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
