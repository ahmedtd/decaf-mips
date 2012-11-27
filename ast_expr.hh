/* File: ast_expr.h
 * ----------------
 * The Expr class and its subclasses are used to represent
 * expressions in the parse tree.  For each expression in the
 * language (add, call, New, etc.) there is a corresponding
 * node class for that construct. 
 *
 * pp5: You will need to extend the Expr classes to implement 
 * code generation for expressions.
 */


#ifndef _H_ast_expr
#define _H_ast_expr

#include <string>
using std::string;

#include "ast.hh"
#include "ast_stmt.hh"

class Type;

class Expr : public Stmt 
{
public:
    Expr(const yyltype &loc);
    Expr();

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const = 0;
};

class EmptyExpr
    : public Expr
{
public:
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class IntConstant
    : public Expr 
{
protected:
    const int value;
  
public:
    IntConstant(const yyltype &loc, int val);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class DoubleConstant
    : public Expr 
{
protected:
    const double value;
    
public:
    DoubleConstant(const yyltype &loc, double val);
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const = 0;
};

class BoolConstant : public Expr 
{
protected:
    const bool value;
    
public:
    BoolConstant(const yyltype &loc, bool val);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class StringConstant : public Expr 
{ 
protected:
    const string value;
    
public:
    StringConstant(const yyltype &loc, const string &in_value);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class NullConstant: public Expr 
{
public: 
    NullConstant(const yyltype &loc);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

// class Operator : public Node 
// {
// protected:
//     string m_token;
    
// public:
//     Operator(const yyltype &loc, const string &token);
//     friend std::ostream& operator<<(std::ostream& out, const Operator &op);

//     // Emit has no meaning for an operator
//     virtual void emit(
//         CodeGenerator &generator,
//         deque<scope> &scope_chain,
//         size_t &num_local_bytes,
//         Location &result_location
//     ) const;
// };

// CompoundExprs might be consolidated just to be one class
// From a machine standpoint, none are different at all, except for assignment
// class CompoundExpr : public Expr
// {
// protected:
//     Operator *m_op;
//     Expr *m_l_expr; // left will be NULL if unary
//     Expr *m_r_expr;
    
// public:
//     CompoundExpr(Expr *lhs, Operator *op, Expr *rhs); // for binary
//     CompoundExpr(Operator *op, Expr *rhs);             // for unary

//     virtual void emit(
//         CodeGenerator &generator,
//         deque<scope> &scope_chain,
//         size_t &num_local_bytes,
//         Location &result_location
//     ) const = 0;
// };

// class ArithmeticExpr : public CompoundExpr 
// {
// public:
//     ArithmeticExpr(Expr *lhs, Operator *op, Expr *rhs);
//     ArithmeticExpr(Operator *op, Expr *rhs);
// };

// class RelationalExpr : public CompoundExpr 
// {
// public:
//     RelationalExpr(Expr *lhs, Operator *op, Expr *rhs);
// };

// class EqualityExpr : public CompoundExpr 
// {
// public:
//     EqualityExpr(Expr *lhs, Operator *op, Expr *rhs);
//     //const char *GetPrintNameForNode() { return "EqualityExpr"; }
// };

// class LogicalExpr : public CompoundExpr 
// {
// public:
//     LogicalExpr(Expr *lhs, Operator *op, Expr *rhs);
//     LogicalExpr(Operator *op, Expr *rhs);
//     //const char *GetPrintNameForNode() { return "LogicalExpr"; }
// };

// class AssignExpr : public CompoundExpr 
// {
// public:
//     AssignExpr(Expr *lhs, Operator *op, Expr *rhs);
//     //const char *GetPrintNameForNode() { return "AssignExpr"; }
// };

// Represents all binary operations
class BinaryExpr
    : public Expr
{
public:
    enum OpCode {
        assign,
        plus,
        minus,
        divide,
        modulus,
        multiply,
        equal,
        not_equal,
        less,
        greater,
        less_eq,
        greater_eq,
        logic_and,
        logic_or
    };

public:
    BinaryExpr(Expr *lop, Expr *rop, const OpCode &op);
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;

protected:
    unique_ptr<Expr> m_lop;
    unique_ptr<Expr> m_rop;
    
    OpCode m_op;
};

class UnaryExpr
    : public Expr
{
public:
    enum OpCode {
        negate_arith,
        negate_logic
    };

public:
    UnaryExpr(Expr *rop, const OpCode &op);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;

protected:
    unique_ptr<Expr> m_rop;

    OpCode m_op;
};

class LValue : public Expr 
{
public:
    LValue(const yyltype &loc);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const = 0;
};

class This : public Expr 
{
public:
    This(const yyltype &loc);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class ArrayAccess : public LValue 
{   
public:
    ArrayAccess(const yyltype &loc, Expr *base, Expr *subscript);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;

    // After calling emit, this holds a stack location with the
    // address corresponding to the access
    
    // the mutable is a quick and dirty hack
    // it should never be used like this
    mutable Location final_addr;

protected:
    unique_ptr<Expr> m_base;
    unique_ptr<Expr> m_subscript;
};

/* Note that field access is used both for qualified names
 * base.field and just field without qualification. We don't
 * know for sure whether there is an implicit "this." in
 * front until later on, so we use one node type for either
 * and sort it out later. */
class FieldAccess : public LValue 
{
protected:
    unique_ptr<Expr> m_base;	// will be NULL if no explicit base
    unique_ptr<Identifier> m_field;
    
public:
    FieldAccess(Expr *base, Identifier *field); //ok to pass NULL base

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
   
    mutable bool is_class_member;
    mutable int offset_from_this;
};

/* Like field access, call is used both for qualified base.field()
 * and unqualified field().  We won't figure out until later
 * whether we need implicit "this." so we use one node type for either
 * and sort it out later. */
class Call : public Expr 
{
protected:
    unique_ptr<Expr> m_base;	// will be NULL if no explicit base
    unique_ptr<Identifier> m_field;
    vector<Expr*> m_actuals;
    
public:
    Call(const yyltype &loc,
         Expr *base,
         Identifier *field,
         vector<Expr*> *args);
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class NewExpr : public Expr
{
protected:
    unique_ptr<Type> m_type;
    
public:
    NewExpr(const yyltype &loc, Type *make_type);
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class NewArrayExpr : public Expr
{
protected:
    unique_ptr<Expr> m_size;
    unique_ptr<Type> m_type;
    
public:
    NewArrayExpr(const yyltype &loc, Expr *size, Type *make_type);

    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class ReadIntegerExpr : public Expr
{
public:
    ReadIntegerExpr(const yyltype &loc);
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

class ReadLineExpr : public Expr
{
public:
    ReadLineExpr(const yyltype &loc);
    
    virtual void emit(
        CodeGenerator &generator,
        deque<scope> &scope_chain,
        size_t &num_local_bytes,
        Location &result_location
    ) const;
};

    
#endif
