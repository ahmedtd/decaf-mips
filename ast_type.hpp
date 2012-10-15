/* File: ast_type.h
 * ----------------
 * In our parse tree, Type nodes are used to represent and
 * store type information. The base Type class is used
 * for built-in types, the NamedType for classes and interfaces,
 * and the ArrayType for arrays of other types.  
 *
 * pp3: You will need to extend the Type classes to implement
 * the type system and rules for type equivalency and compatibility.
 */
 
#ifndef _H_ast_type
#define _H_ast_type

#include <iostream>
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;

#include "ast.hpp"

// class decaf_type
//     : public Node
// {
// public:
//     decaf_type(const yyltype &loc, const string &name);
    
// protected:
//     string m_name;
//     bool m_is_array;
//     vector<shared_ptr<decaf_type> > m_args;
//     shared_ptr<decaf_type> m_return;
// }

class Type : public Node 
{
public :
    // static Type *intType, *doubleType, *boolType, *voidType,
    //     *nullType, *stringType, *errorType;

    Type(const yyltype loc);
    Type(Identifier *ident);
    
    virtual ostream& stream_print(ostream& out) const;

    const Identifier& ident() const;

    // Strip any array qualifiers from the type
    virtual const Type& bare() const;

protected:
    const Identifier *m_ident;
};

class ArrayType : public Type 
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);
    
    virtual ostream& stream_print(ostream& out) const;
    
    virtual const Type& bare() const;
};

ostream& operator<<(ostream& out, const Type &t);
 
#endif
