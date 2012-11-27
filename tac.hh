/* File: tac.h
 * -----------
 * This module contains the Instruction class (and its various
 * subclasses) that represent Tac instructions and the Location
 * class used for operands to those instructions.
 *
 * Each instruction is mostly just a little struct with a
 * few fields, but each responds polymorphically to the methods
 * Print and Emit, the first is used to print out the TAC form of
 * the instruction (helpful when debugging) and the second to
 * convert to the appropriate MIPS assembly.
 *
 * The operands to each instruction are of Location class.
 * A Location object is a simple representation of where a variable
 * exists at runtime, i.e. whether it is on the stack or global
 * segment and at what offset relative to the current fp or gp.
 *
 * You may need to make changes/extensions to these classes
 * if you are working on IR optimization.

 */

#ifndef _H_tac
#define _H_tac

#include <ostream>
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "ast_type.hh"

class Mips;


// A Location object is used to identify the operands to the
// various TAC instructions. A Location is either fp or gp
// relative (depending on whether in stack or global segemnt)
// and has an offset relative to the base of that segment.
// For example, a declaration for integer num as the first local
// variable in a function would be assigned a Location object
// with name "num", segment fpRelative, and offset -8. 
 
typedef enum {fpRelative, gpRelative} Segment;

class Location
{
protected:
    string m_varname;
    Segment m_segment;
    int m_offset;
    bool m_bound;
    Type m_type;

public:
    Location();
    Location(Segment seg, int offset, const string &name, const Type &type);

    Location& operator=(const Location &other);

    const string& name() const;
    const Segment& segment() const;
    const int offset() const;
    const Type& type() const;

    void segment(const Segment &newseg);
};
 
// base class from which all Tac instructions derived
// has the interface for the 2 polymorphic messages: Print & Emit
  
class Instruction
{	  
public:
	virtual void Print(ostream &out) const = 0;
	virtual void EmitSpecific(Mips *mips) const = 0;
	virtual void Emit(Mips *mips) const;
};

// for convenience, the instruction classes are listed here.
// the interfaces for the classes follows below
  
class LoadConstant;
class LoadStringConstant;
class LoadLabel;
class Assign;
class Load;
class Store;
class BinaryOp;
class Label;
class Goto;
class IfZ;
class BeginFunc;
class EndFunc;
class Return;
class PushParam;
class RemoveParams;
class LCall;
class ACall;
class VTable;

class LoadConstant: public Instruction
{
    Location dst;
    int val;

public:
    LoadConstant(const Location &dst, int val);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class LoadStringConstant: public Instruction {
    Location dst;
    string str;
public:
    LoadStringConstant(const Location &d, const string &s);
    
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};
    
class LoadLabel: public Instruction {
    Location dst;
    const string label;
public:
    LoadLabel(const Location &d, const string &l);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class Assign: public Instruction {
    Location dst;
    Location src;
public:
    Assign(const Location &d, const Location &s);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class Load: public Instruction {
    Location dst;
    Location src;
    int offset;
public:
    Load(const Location &d,
         const Location &s,
         int offset = 0);
    
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class Store: public Instruction {
    Location dst;
    Location src;
    int offset;
public:
    Store(const Location &d, const Location &s, int offset = 0);
    
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class BinaryOp: public Instruction {

public:
    typedef enum {Add, Sub, Mul, Div, Mod, Eq, Less, And, Or, NumOps} OpCode;
    static const char * const opName[NumOps];
    
    //static OpCode OpCodeForName(const char *name);
    
protected:
    OpCode code;
    Location dst;
    Location op1;
    Location op2;
public:
    BinaryOp(OpCode c,
             const Location &d,
             const Location &o1,
             const Location &o2);
    
    virtual void Print(ostream &out) const;

    virtual void EmitSpecific(Mips *mips) const;
};

class Label: public Instruction
{
    const string label;
public:
    Label(const string &in_label);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class Goto: public Instruction {
    const string label;
public:
    Goto(const string &in_label);
    
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class IfZ: public Instruction
{
    Location test;
    const string label;
public:
    IfZ(const Location &t, const string &l);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class BeginFunc: public Instruction
{
    int frameSize;
public:
    BeginFunc();
    // used to backpatch the instruction with frame size once known
    void SetFrameSize(int numBytesForAllLocalsAndTemps);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class EndFunc: public Instruction {
public:
    EndFunc();

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class ReturnVoid
    : public Instruction
{
public:
    ReturnVoid();
    void Print(ostream &out);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};

class ReturnVal: public Instruction
{
    Location val;
    
public:
    ReturnVal(const Location &val);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};   

class PushParam: public Instruction {
    Location param;
public:
    PushParam(const Location &p);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
}; 

class PopParams: public Instruction {
    int numBytes;
public:
    PopParams(int numBytesOfParamsToRemove);

    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
}; 

class LCallVoid
    : public Instruction
{
public:
    LCallVoid(const string &in_label);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
private:
    const string label;
};

class LCallVal
    : public Instruction
{
public:
    LCallVal(const string &in_label, const Location &in_result);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;

private:
    const string label;
    const Location dst;
};

class ACallVoid
    : public Instruction
{
public:
    ACallVoid(const Location &meth);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
private:
    const Location methodAddr;
};

class ACallVal
    : public Instruction
{
public:
    ACallVal(const Location &method, const Location &result);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
private:
    const Location result;
    const Location methodAddr;
};

class VTable
    : public Instruction
{
    vector<string> methodLabels;
    const string label;
public:
    VTable(const string &in_label, const vector<string> &in_methodLabels);
    virtual void Print(ostream &out) const;
    virtual void EmitSpecific(Mips *mips) const;
};


#endif
