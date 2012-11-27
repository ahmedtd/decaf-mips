/* File: ast_type.h
 * ----------------
 * In our parse tree, Type nodes are used to represent and
 * store type information. The base Type class is used
 * for built-in types, the NamedType for classes and interfaces,
 * and the ArrayType for arrays of other types.  
 *
 * pp5: You will need to extend the Type classes to implement
 * code generation for types.
 */
 
#ifndef _H_ast_type
#define _H_ast_type

#include <ostream>
#include <string>
using std::string;

#include "ast.hh"

class Type : public Node 
{
protected:
    string m_base_name;
    size_t m_array_degree;

public :
    static Type &decaf_int;
    static Type &decaf_double;
    static Type &decaf_bool;
    static Type &decaf_void;
    static Type &decaf_null;
    static Type &decaf_string;
    static Type &decaf_error;
    static Type &decaf_label;

protected:
    static Type def_decaf_int;
    static Type def_decaf_double;
    static Type def_decaf_bool;
    static Type def_decaf_void;
    static Type def_decaf_null;
    static Type def_decaf_string;
    static Type def_decaf_error;
    static Type def_decaf_label;

public:
    Type(const Type &other);
    
    /// Construct a type with a default location -- suitable for builtin types
    Type(const string &base_name);

    /// Construct a named type with a location where it's specified
    Type(const string &base_name, const yyltype &loc);
    
    /// Construct a named array type with a location
    Type(const string &base_name,
         const yyltype &loc,
         const size_t array_degree);

    const string& base_name() const;
    const size_t array_degree() const;

    Type array_stored() const;
    Type array_wrap() const;
    
    virtual bool operator==(const Type &other) const;

    friend std::ostream& operator<<(std::ostream& out, const Type &to_print);
    
    
};

#endif
