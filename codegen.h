/* File: codegen.h
 * ---------------
 * The CodeGenerator class defines an object that will build Tac
 * instructions (using the Tac class and its subclasses) and store the
 * instructions in a sequential list, ready for further processing or
 * translation to MIPS as part of final code generation.
 */

#ifndef _H_codegen
#define _H_codegen

#include <cstdlib>
#include <list>
#include "tac.h"

#include "target.hh"

class FnDecl;

enum class decaf_built_in {
    Alloc, ReadLine, ReadInteger, StringEqual, PrintInt, PrintString, PrintBool, Halt };

class CodeGenerator
{
public:
    // Here are some class constants to remind you of the offsets
    // used for globals, locals, and parameters. You will be
    // responsible for using these when assigning Locations.
    // In a MIPS stack frame, first local is at fp-8, subsequent locals
    // are at fp-12, fp-16, and so on. The first param is at fp+4,
    // subsequent ones as fp+8, fp+12, etc. (Because methods have secret
    // "this" passed in first param slot at fp+4, all normal params
    // are shifted up by 4.)  First global is at offset 0 from global
    // pointer, all subsequent at +4, +8, etc.
    // Conveniently, all vars are 4 bytes in size for code generation
    static const int OffsetToFirstLocal = -8;
    static const int OffsetToFirstParam = 4;
    static const int OffsetToFirstGlobal = 0;
    static const int VarSize = 4;

    // When a this-ptr is present, it is the first parameter to a function
    // A this-expression will return this target
    target target_this;

    CodeGenerator();
    
    // Assigns a new unique label name and returns it. Does not
    // generate any Tac instructions (see GenLabel below if needed)
    string NewLabel();

    // Place the given comment string into the source
    void GenComment(const string &comment);

    // Creates and returns a Location for a new uniquely named
    // temp variable. Does not generate any Tac instructions
    target GenTempVar(const string &descriptor);

    // Create a new target for a local variable
    //
    // The returned target will be with the given name and subscript 0
    target GenLocalVariable(const string &name);
    
    // Create a target for a global variable
    //
    // At this point, I'm not sure if a global should use its subscripts or not
    target GenGlobalVariable(const string &name);
    
    // Generates Tac instructions to load a constant value. Creates
    // a new temp var to hold the result. The constant 
    // value is passed as an integer, it can be 0 for integer zero,
    // false for bool, NULL for null object, etc. All are just 4-byte
    // zero in the code generation world.
    // The second overloaded version is used for string constants.
    // The LoadLabel method loads a label into a temporary.
    // Each of the methods returns a Location for the temp var
    // where the constant was loaded.

    // Load a constant value into a target
    target GenLoadConstant(int value,
                           const string &descriptor);

    // Load a constant string value into a target
    target GenLoadConstant(const string &value,
                           const string &descriptor);
    
    // Load the address of a label into a target
    target GenLoadLabel(const string &label,
                        const string &descriptor);


    // Generates Tac instructions to copy value from one location to another
    void GenAssign(const target &dst,
                   const target &src,
                   const string &descriptor);

    // Generates Tac instructions to dereference addr and store value
    // into that memory location. addr should hold a valid memory address
    // (most likely computed from an array or field offset calculation).
    // The optional offset argument can be used to offset the addr by a
    // positive/negative number of bytes. If not given, 0 is assumed.
    void GenStore(const target &addr,
                  const target &value,
                  int offset = 0,
                  const string &descriptor = "");

    // Generates Tac instructions to dereference addr and load contents
    // from a memory location into a new temp var. addr should hold a
    // valid memory address (most likely computed from an array or
    // field offset calculation). Returns the Location for the new
    // temporary variable where the result was stored. The optional
    // offset argument can be used to offset the addr by a positive or
    // negative number of bytes. If not given, 0 is assumed.
    target GenLoad(const target &addr,
                   int offset = 0,
                   const string descriptor = "");

    
    // Generates Tac instructions to perform one of the binary ops
    // identified by string name, such as "+" or "==".  Returns a
    // Location object for the new temporary where the result
    // was stored.
    target GenBinaryOp(const BinaryOp::OpCode &operation,
                       const target &left_operand,
                       const target &right_operand,
                       const string &descriptor);

    // Generates the Tac instructions for a LCall, a jump to
    // a compile-time label. The params to the target routine
    // should already have been pushed. If hasReturnValue is
    // true,  a new temp var is created, the fn result is stored 
    // there and that Location is returned. If false, no temp is
    // created and NULL is returned
    Location *GenLCall(const char *label, bool fnHasReturnValue);

    // 
    Location *GenACall(Location *fnAddr, bool fnHasReturnValue);
    
    // These methods generate the Tac instructions for various
    // control flow (branches, jumps, returns, labels)
    // One minor detail to mention is that you can pass NULL
    // (or omit arg) to GenReturn for a return that does not
    // return a value
    void GenIfZ(Location *test, const char *label);
    void GenGoto(const char *label);
    void GenReturn(Location *val = NULL);
    void GenLabel(const char *label);


    // These methods generate the Tac instructions that mark the start
    // and end of a function/method definition. 
    BeginFunc *GenBeginFunc(FnDecl *fn);
    void GenEndFunc();

    
    // Generates the Tac instructions for defining vtable for a
    // The methods parameter is expected to contain the vtable
    // methods in the order they should be laid out.  The vtable
    // is tagged with a label of the class name, so when you later
    // need access to the vtable, you use LoadLabel of class name.
    void GenVTable(const char *className, List<const char*> *methodLabels);


    // Emits the final "object code" for the program by
    // translating the sequence of Tac instructions into their mips
    // equivalent and printing them out to stdout. If the debug
    // flag tac is on (-d tac), it will not translate to MIPS,
    // but instead just print the untranslated Tac. It may be
    // useful in debugging to first make sure your Tac is correct.
    void DoFinalCodeGen();

    Location *GenNewArray(Location *numElements);
    Location *GenArrayLen(Location *array);
    Location *GenNew(const char *vTableLabel, int instanceSize);
    Location *GenDynamicDispatch(Location *obj, int vtableOffset, List<Location*> *args, bool hasReturnValue);
    Location *GenSubscript(Location *array, Location *index);
    
    void GenHaltWithMessage(const char *msg);

private:
    vector<unique_ptr<Instruction*>> code;
    int curStackOffset;
    int curGlobalOffset;
    BeginFunc *insideFn;

    // The root temporary variable target, labelled 
    target target_tmp;

    // The special target for register return values
    target reg_return_target;
};

#endif
