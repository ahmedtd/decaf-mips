/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <cassert>
#include <cstring>

#include "ast_expr.hpp"
#include "ast_type.hpp"
#include "ast_decl.hpp"

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

StringConstant::StringConstant(yyltype loc, const char *val)
    : Expr(loc)
{
    assert(val != NULL);
    value = strdup(val);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
    : Expr(Join(l->location, r->location)),
      op(o),
      left(l),
      right(r)
{
    assert(l != NULL && o != NULL && r != NULL);
    // (op=o)->SetParent(this);
    // (left=l)->SetParent(this); 
    // (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
    : Expr(Join(o->location, r->location)),
      op(o),
      left(nullptr),
      right(r)
{
    assert(o != NULL && r != NULL);
    // left = NULL; 
    // (op=o)->SetParent(this);
    // (right=r)->SetParent(this);
}
   
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s)
    : LValue(loc),
      base(b),
      subscript(s)
{
    // (base=b)->SetParent(this); 
    // (subscript=s)->SetParent(this);
}
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
    : LValue(b? Join(b->location, f->location) : f->location),
      base(b),
      field(f)
{
    assert(f != NULL); // b can be be NULL (just means no explicit base)
    // base = b; 
    // if (base) base->SetParent(this); 
    // (field=f)->SetParent(this);
}


Call::Call(yyltype loc, Expr *b, Identifier *f, vector<Expr*> *a)
    : Expr(loc),
      base(b),
      field(f),
      actuals(a)
{
    assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)

    // if (base) base->SetParent(this);
    // field->SetParent(this);

    // for(auto actp : *actuals)
    // {
    //     actp->SetParent(this);
    // }
}
 

NewExpr::NewExpr(yyltype loc, NamedType *c)
    : Expr(loc),
      cType(c)
{ 
  assert(c != NULL);
  // (cType=c)->SetParent(this);
}


NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et)
    : Expr(loc),
      size(sz),
      elemType(et)
{
    assert(sz != NULL && et != NULL);
    // (size=sz)->SetParent(this); 
    // (elemType=et)->SetParent(this);
}

       
