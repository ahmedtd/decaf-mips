
#include "scope.hpp"

basic_scope::basic_scope()
{
    
}

basic_scope::const_iterator basic_scope::cbegin() const
{
    return begin(m_decls_at_scope);
}

basic_scope::const_iterator basic_scope::cend() const
{
    return end(m_decls_at_scope);
}

basic_scope::const_iterator basic_scope::find(const string &find_name) const
{
    return m_decls_at_scope.find(find_name);
}
