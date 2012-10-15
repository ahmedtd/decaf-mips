/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */

#include <cassert>
#include <cstring>

#include "ast_type.hpp"
#include "ast_decl.hpp"
 
/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error"); 

Type::Type(const char *n) {
    assert(n);
    typeName = strdup(n);
}
	
NamedType::NamedType(Identifier *i)
    : Type(i->location),
      id(i)
{
    assert(i != NULL);
    //(id=i)->parent(this);
} 


ArrayType::ArrayType(yyltype loc, Type *et)
    : Type(loc),
      elemType(et)
{
    assert(et != NULL);
    //(elemType=et)->parent(this);
}

