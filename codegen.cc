/* File: codegen.cc
 * ----------------
 * Implementation for the CodeGenerator class. The methods don't do anything
 * too fancy, mostly just create objects of the various Tac instruction
 * classes and append them to the list.
 */

#include "codegen.h"
#include <string.h>
#include "tac.h"
#include "mips.h"
#include "ast_decl.h"
#include "errors.h"
#include "cfg.h"
  
CodeGenerator::CodeGenerator()
{
    curGlobalOffset = 0;
}

string CodeGenerator::NewLabel(const string &descriptor)
{
    static int nextLabelNum = 0;
    stringstream labelstream;
    labelstream << "label_" << descriptor << "_" << nextlabel++;
    return labelstream.str();
}

void CodeGenerator::GenComment(
    const string &comment,
    const Comment::style &style = Comment::style::full_line)
{
    code.push_back(new Comment(comment, style));
}

target CodeGenerator::GenTempVar(const string &descriptor)
{
    target return_target = target_tmp;
    target_tmp = target_tmp.subsequent(descriptor);
    return return_target;
}

// Create a new target for a local variable
//
// This target will be the root of a possible target chain.  It's important to
// note that not all targets will end up corresponding to a memory location,
// especially if they are highlighted by the liveness analysis as dead.
target CodeGenerator::GenLocalVariable(const string &name)
{
    return target(name, target::location_type::local);
}

target CodeGenerator::GenGlobalVariable(const string &name)
{
    return target(name, target::location_type::global);
}

target CodeGenerator::GenLoadConstant(int value, const string &descriptor)
{
    target result_target = GenTempVar(descriptor);


    // Output the TAC for the constant load
    code.push_back(new LoadConstant(result, value, descriptor));
    
    return result;
}

target CodeGenerator::GenLoadConstant(const string &value
                                      const string &descriptor)
{
    target result_target = GenTempVar(descriptor);
    
    code.push_back(new LoadStringConstant(result, value, descriptor));
    
    return result_target;
} 

target CodeGenerator::GenLoadLabel(const string &label,
                                   const string &descriptor)
{
    target result_target = GenTempVar(descriptor);

    code.push_back(new LoadLabel(result, label, descriptor));

    return result;
} 

void CodeGenerator::GenAssign(const target &dst,
                              const target &src,
                              const string &descriptor)
{
    code.push_back(new Assign(dst, src, descriptor));
}

void CodeGenerator::GenStore(const target &addr,
                             const target &value,
                             const int offset,
                             const string &descriptor)
{
    code.push_back(new Store(addr, value, offset, descriptor));
}

target CodeGenerator::GenLoad(const target &addr,
                              const int offset)
{
    target result_target = GenTempVar(descriptor);
    code.push_back(new Load(result_target, addr, offset));
    return result_target;
}

target CodeGenerator::GenBinaryOp(
    const BinaryOp::OpCode &operation,
    const target &left_operand,
	const target &right_operand,
    const string &descriptor)
{
    target result_target = GenTempVar(descriptor);
    
    code.push_back(
        new BinaryOp(operation,
                     result_target,
                     left_operand,
                     right_operand));
    
    return result_target;
}

// void CodeGenerator::GenPushParam(const target &param, const string &descriptor)
// {
//     code.push_back(new PushParam(param));
// }

// void CodeGenerator::GenPopParams(int numBytesOfParams)
// {
//   Assert(numBytesOfParams >= 0 && numBytesOfParams % VarSize == 0); // sanity check
//   if (numBytesOfParams > 0)
//     code.push_back(new PopParams(numBytesOfParams));
// }

template<class Iterator>
target CodeGenerator::GenLCall(const string &label,
                               const Iterator &args_begin,
                               const Iterator &args_end)
{
    // A call will always put its result in the return register

    code.push_back(
        new LCall(
            label,
            reg_return_target,
            args_begin,
            args_end
        )
    );

    return reg_return_target;
}

template<class Iterator>
target CodeGenerator::GenACall(const target &function_address,
                               const Iterator &args_begin,
                               const Iterator &args_end)
{
    // A call will always put its result in the return register
    
    code.push_back(
        new ACall(
            fnAddr,
            reg_return_target));
  
    return reg_return_target;
}

target CodeGenerator::GenAlloc(
    const target &size_target)
{
    vector<target> argvec = {size_target};
    return GenLCall("_Alloc", begin(argvec), end(argvec));
}

target CodeGenerator::GenReadLine()
{
    vector<target> argvec;
    return GenLCall("_ReadLine", begin(argvec), end(argvec));
}

target CodeGenerator::GenReadInteger()
{
    vector<target> argvec;
    return GenLCall("_ReadInteger", begin(argvec), end(argvec));
}

target CodeGenerator::GenStringEqual(const target &left_target,
                                     const target &right_target)
{
    vector<target> argvec = { left_target, right_target };
    return GenLCall("_StringEqual", begin(argvec), end(argvec));
}

target CodeGenerator::GenPrintInt(const target &print_target)
{
    vector<target> argvec = { print_target };
    return GenLCall("_PrintInt", begin(argvec), end(argvec));
}

target CodeGenerator::GenPrintString(const target &print_target)
{
    vector<target> argvec = { print_target };
    return GenLCall("_PrintString", begin(argvec), end(argvec));
}

target CodeGenerator::GenPrintBool(const target &print_target)
{
    vector<target> argvec = { print_target };
    return GenLCall("_PrintBool", begin(argvec), end(argvec));
}

target CodeGenerator::GenHalt()
{
    vector<target> argvec = {};
    return GenLCall("_Halt", begin(argvec), end(argvec));
}

void CodeGenerator::GenIfZ(const target &test,
                           const string &dst_label,
                           const string &descriptor)
{
    code.push_back(new IfZ(test, dst_label, descriptor));
}

void CodeGenerator::GenGoto(const string &label, const string &descriptor)
{
    code.push_back(new Goto(label, descriptor));
}

void CodeGenerator::GenReturn(const target &return_target)
{
    code.push_back(new Return(return_target));
}

void CodeGenerator::GenLabel(const string &label, const string &descriptor)
{
    code.push_back(new Label(label, descriptor));
}

template<class Iterator>
void CodeGenerator::GenBeginFunc(const Iterator &formals_begin,
                                 const Iterator &formals_end)
{
    // Iterator iterates over VarDecl*

    // Create parameter-class targets for each formal
    for(auto it = formals_begin; it != formals_end; ++it)
    {
        (*it)->target = (target(
                             (*it)->GetName(),
                             0,
                             target::value_type::lvalue,
                             target::location_type::parameter,
                             0
                         ));
    }

    BeginFunc *tac_instr = new BeginFunc(

    // Set insidefn to the generated BeginFunc instruction

    // BeginFunc *result = new BeginFunc;
    // code.push_back(insideFn = result);
    // List<VarDecl*> *formals = fn->GetFormals();
    // int start = OffsetToFirstParam;
    // if (fn->IsMethodDecl()) start += VarSize;
    // for (int i = 0; i < formals->NumElements(); i++)
    //   formals->Nth(i)->rtLoc = new Location(fpRelative, i*VarSize + start, formals->Nth(i)->GetName());
    // curStackOffset = OffsetToFirstLocal;
    // return result;
}

void CodeGenerator::GenEndFunc()
{
    if(insideFn == nullptr)
        throw logic_error(
            "CodeGenerator::GenEndFunc(): Called without a matching call to"
            " GenEndFunc().");

    code.push_back(new EndFunc());

    // Patch the current function
    insideFn->SetFrameSize(OffsetToFirstLocal-curStackOffset);
    insideFn = nullptr;
}




void CodeGenerator::GenVTable(const char *className, List<const char *> *methodLabels)
{
  code.push_back(new VTable(className, methodLabels));
}


void CodeGenerator::DoFinalCodeGen()
{
  if (IsDebugOn("tac")) { // if debug don't translate to mips, just print Tac
    std::list<Instruction*>::iterator p;
    for (p= code.begin(); p != code.end(); ++p) {
      (*p)->Print();
    }
  } else {
    Mips mips;
    mips.EmitPreamble();
    std::list<Instruction*>::iterator p= code.begin(),
                                      begin_func= code.end(),
                                      end_func= code.end();
    while (p != code.end())
    {
      // Watch for beginning of function
      if (dynamic_cast<BeginFunc*>(*p))
      {
        begin_func= p;

        // Skip to the end
        do ++p; while (p != code.end() && !dynamic_cast<EndFunc*>(*p));
        if (p == code.end())
          p= begin_func;
        else
        {
          end_func= p;

          ControlFlowGraph cfg(begin_func, end_func);
          // TODO (phase 2): use CFG for liveness analysis

          // Now go back and print out the instructions
          for (p= begin_func; p != end_func; ++p)
            (*p)->Emit(&mips);
        }
      }

      (*p)->Emit(&mips);
      ++p;
    }
  }
}



Location *CodeGenerator::GenArrayLen(Location *array)
{
  return GenLoad(array, -4);
}

Location *CodeGenerator::GenNew(const char *vTableLabel, int instanceSize)
{
  Location *size = GenLoadConstant(instanceSize);
  Location *result = GenBuiltInCall(Alloc, size);
  Location *vt = GenLoadLabel(vTableLabel);
  GenStore(result, vt);
  return result;
}


Location *CodeGenerator::GenDynamicDispatch(Location *rcvr, int vtableOffset, List<Location*> *args, bool hasReturnValue)
{
  Location *vptr = GenLoad(rcvr); // load vptr
  Assert(vtableOffset >= 0);
  Location *m = GenLoad(vptr, vtableOffset*4);
  return GenMethodCall(rcvr, m, args, hasReturnValue);
}

// all variables (ints, bools, ptrs, arrays) are 4 bytes in for code generation
// so this simplifies the math for offsets
Location *CodeGenerator::GenSubscript(Location *array, Location *index)
{
  Location *zero = GenLoadConstant(0);
  Location *isNegative = GenBinaryOp("<", index, zero);
  Location *count = GenLoad(array, -4);
  Location *isWithinRange = GenBinaryOp("<", index, count);
  Location *pastEnd = GenBinaryOp("==", isWithinRange, zero);
  Location *outOfRange = GenBinaryOp("||", isNegative, pastEnd);
  const char *pastError = NewLabel();
  GenIfZ(outOfRange, pastError);
  GenHaltWithMessage(err_arr_out_of_bounds);
  GenLabel(pastError);
  Location *four = GenLoadConstant(VarSize);
  Location *offset = GenBinaryOp("*", four, index);
  Location *elem = GenBinaryOp("+", array, offset);
  return new Location(elem, 0); 
}



Location *CodeGenerator::GenNewArray(Location *numElems)
{
  Location *zero = GenLoadConstant(0);
  Location *isNegative = GenBinaryOp("<", numElems, zero);
  const char *pastError = NewLabel();
  GenIfZ(isNegative, pastError);
  GenHaltWithMessage(err_arr_bad_size);
  GenLabel(pastError);

  // need (numElems+1)*VarSize total bytes (extra 1 is for length)
  Location *arraySize = GenLoadConstant(1);
  Location *num = GenBinaryOp("+", arraySize, numElems);
  Location *four = GenLoadConstant(VarSize);
  Location *bytes = GenBinaryOp("*", num, four);
  Location *result = GenBuiltInCall(Alloc, bytes);
  GenStore(result, numElems);
  return GenBinaryOp("+", result, four);
}


void CodeGenerator::GenHaltWithMessage(const char *message)
{
   Location *msg = GenLoadConstant(message);
   GenBuiltInCall(PrintString, msg);
   GenBuiltInCall(Halt, NULL);
}
   

 
