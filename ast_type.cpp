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

// Type *Type::intType    = new Type("int");
// Type *Type::doubleType = new Type("double");
// Type *Type::voidType   = new Type("void");
// Type *Type::boolType   = new Type("bool");
// Type *Type::nullType   = new Type("null");
// Type *Type::stringType = new Type("string");
// Type *Type::errorType  = new Type("error"); 

Type::Type(Identifier *ident)
    : Node(ident->location),
      m_ident(ident)
{
}

ostream& Type::stream_print(ostream &out) const
{
    return out << m_ident->name;
}

const Identifier& Type::ident() const
{
    return *m_ident;
}

const Type& Type::bare() const
{
    return *(this);
}

ArrayType::ArrayType(yyltype loc, Type *et)
    : Type(new Identifier(loc, et->ident().name)),
//    : Type(new Identifier(et->location, et->ident().name)),
      elemType(et)
{
    assert(et != NULL);
    //(elemType=et)->parent(this);
}

ostream& ArrayType::stream_print(ostream &out) const
{
    return out << m_ident->name << "[]";
}

const Type& ArrayType::bare() const
{
    return *(elemType);
}

bool operator==(const Type &rop, const Type &lop)
{
    const ArrayType* rop_arrayp = dynamic_cast<const ArrayType*>(&rop);
    const ArrayType* lop_arrayp = dynamic_cast<const ArrayType*>(&lop);

    if(rop_arrayp && lop_arrayp)
    {
        // If they're both arrays

        if(lop_arrayp->elemType->ident().name == rop_arrayp->elemType->ident().name)
        {
            return true;
        }

        return false;
    }
    else if(!rop_arrayp && !lop_arrayp)
    {
        // If they're both not arrays

        if(rop.ident().name == lop.ident().name)
        {
            return true;
        }
        
        return false;
    }

    return false;
}

ostream& operator<<(ostream &out, const Type &t)
{
    return t.stream_print(out);
}


