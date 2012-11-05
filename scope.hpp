
#ifndef DECAF_SCOPE_HPP
#define DECAF_SCOPE_HPP

#include <deque>
using std::deque;
#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

namespace decaf_compile
{

class basic_scope
{
public:
    typedef string scope_key;

public:
    basic_scope(const basic_scope &copy_scope);

    template<class Iterator, class ErrFunc>
    void insert(
        Iterator begin_decls,
        Iterator end_decls,
        ErrFunc error_function
    );

    template<class ErrFunc>
    void insert(
        const shared_ptr &declaration
    );
    
private:
    unordered_map<scope_key, shared_ptr<class_declaration> > m_declarations_at_scope;
    deque<const shared_ptr<basic_scope> > m_exterior_scope_stack;
};

class scope_resolvable
{
public:
    const basic_scope::scope_key resolution_key() const;
};

}

// class Decl;
// class ClassDecl;
// class InterfaceDecl;

// class basic_scope
// {
// public:
//     enum class type_selector
//     {
//         any,
//         class_only,
//         interface_only
//     };

//     typedef typename map<const string, const Decl*>::iterator iterator;
//     typedef typename
//     map<const string, const Decl*>::const_iterator const_iterator;
    
// public:

//     // Construct an empty scope
//     basic_scope();

//     // Construct a scope with Decls from the given range
//     template<class LocalScopeIterator, class ReportingFunc>
//     basic_scope(
//         LocalScopeIterator begin_local_scope,
//         LocalScopeIterator end_local_scope,
//         ReportingFunc rep_func,
//         const basic_scope &exterior_scope
//     );

//     // ReportingFunc takes a (const Type &) parameter
//     template <class TypeIterator, class ReportingFunc>
//     map<const string, const Decl*> types_exist(
//         TypeIterator begin_types,
//         TypeIterator end_types,
//         ReportingFunc rep_func,
//         type_selector restriction = type_selector::any
//     ) const;

//     const_iterator cbegin() const;
//     const_iterator cend() const;

//     const_iterator find(const string &find_name) const;

// protected:
//     map<const string, const Decl*> m_decls_at_scope;
// };

// template<class LocalScopeIterator,
//          class ReportingFunc>
// basic_scope::basic_scope(
//     LocalScopeIterator begin_local_scope,
//     LocalScopeIterator end_local_scope,
//     ReportingFunc rep_func,
//     const basic_scope &exterior_scope
// )
// {
//     // Insert declarations from the program scope, calling the reporting
//     // function on any conflicts
    
//     for(; begin_local_scope != end_local_scope; ++begin_local_scope)
//     {
//         auto declp = *begin_local_scope;

//         auto insert_info = m_decls_at_scope.insert(
//             make_pair(declp->ident().name, declp)
//         );

//         bool insertion_succeeded = insert_info.second;
//         auto insertion_it = insert_info.first;

//         if(insertion_succeeded == false)
//         {
//             rep_func(*declp, *((*insertion_it).second));
//         }
//     }

//     // After reporting any errors in the local scope, fill in all unshadowed
//     // names from the exterior scope
//     m_decls_at_scope.insert(begin(exterior_scope.m_decls_at_scope),
//                             end(exterior_scope.m_decls_at_scope));
// }

// template <class TypeIterator, class ReportingFunc>
// map<const string, const Decl*> basic_scope::types_exist(
//     TypeIterator begin_types,
//     TypeIterator end_types,
//     ReportingFunc rep_func,
//     type_selector restriction
// ) const
// {
//     // Build the set of possible types based on the type selector
//     map<const string, const Decl*> respondent_types;
//     // Add all the classes at the current scope
//     for(auto declpair : m_decls_at_scope)
//     {
//         auto declp = declpair.second;
//         const string &decl_name = declpair.first;

//         const ClassDecl *cdeclp = dynamic_cast<const ClassDecl*>(declp);
//         const InterfaceDecl *ideclp = dynamic_cast<const InterfaceDecl*>(declp);

//         if((restriction == type_selector::any
//             || restriction == type_selector::class_only)
//            && cdeclp)
//         {
//             respondent_types.insert(make_pair(decl_name, declp));
//         }
//         if((restriction == type_selector::any
//             || restriction == type_selector::interface_only)
//            && ideclp)
//         {
//             respondent_types.insert(make_pair(decl_name, declp));
//         }
//         if(restriction == type_selector::any)
//         {
//             map<string, Decl*> basic_types
//                 = {{"int", nullptr},
//                    {"double", nullptr},
//                    {"bool", nullptr},
//                    {"void", nullptr},
//                    {"null", nullptr},
//                    {"string", nullptr},
//                    {"error", nullptr}};
            
//             respondent_types.insert(begin(basic_types),
//                                     end(basic_types));
//         }
//     }
    
//     map<const string, const Decl*> found_decls;

//     // Check if each type is in the respondent type vector
//     for(; begin_types != end_types; ++begin_types)
//     {
//         const string &request_name = (*begin_types)->bare().ident().name;
        
//         auto found_decl = respondent_types.find(request_name);

//         if(found_decl == end(respondent_types))
//         {
//             rep_func((*begin_types)->bare());
//         }
//         else
//         {
//             found_decls.insert(
//                 make_pair(
//                     (*found_decl).first,
//                     (*found_decl).second
//                 )
//             );
//         }
//     }
    
//     return found_decls;
// }

// typedef basic_scope scope;

#endif
