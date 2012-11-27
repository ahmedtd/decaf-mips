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

Location* CodeGenerator::ThisPtr= new Location(fpRelative, 4, "this");
  
CodeGenerator::CodeGenerator()
{
  curGlobalOffset = 0;
}

char *CodeGenerator::NewLabel()
{
  static int nextLabelNum = 0;
  char temp[10];
  sprintf(temp, "_L%d", nextLabelNum++);
  return strdup(temp);
}


Location *CodeGenerator::GenTempVar()
{
  static int nextTempNum;
  char temp[10];
  Location *result = NULL;
  sprintf(temp, "_tmp%d", nextTempNum++);
  return GenLocalVariable(temp);
}

  
Location *CodeGenerator::GenLocalVariable(const char *varName)
{            
    curStackOffset -= VarSize;
    return new Location(fpRelative, curStackOffset+4,  varName);
}

Location *CodeGenerator::GenGlobalVariable(const char *varName)
{
    curGlobalOffset += VarSize;
    return new Location(gpRelative, curGlobalOffset -4, varName);
}


Location *CodeGenerator::GenLoadConstant(int value)
{
  Location *result = GenTempVar();
  code.push_back(new LoadConstant(result, value));
  return result;
}

Location *CodeGenerator::GenLoadConstant(const char *s)
{
  Location *result = GenTempVar();
  code.push_back(new LoadStringConstant(result, s));
  return result;
} 

Location *CodeGenerator::GenLoadLabel(const char *label)
{
  Location *result = GenTempVar();
  code.push_back(new LoadLabel(result, label));
  return result;
} 


void CodeGenerator::GenAssign(Location *dst, Location *src)
{
  code.push_back(new Assign(dst, src));
}


Location *CodeGenerator::GenLoad(Location *ref, int offset)
{
  Location *result = GenTempVar();
  code.push_back(new Load(result, ref, offset));
  return result;
}

void CodeGenerator::GenStore(Location *dst,Location *src, int offset)
{
  code.push_back(new Store(dst, src, offset));
}


Location *CodeGenerator::GenBinaryOp(const char *opName, Location *op1,
						     Location *op2)
{
  Location *result = GenTempVar();
  code.push_back(new BinaryOp(BinaryOp::OpCodeForName(opName), result, op1, op2));
  return result;
}


void CodeGenerator::GenLabel(const char *label)
{
  code.push_back(new Label(label));
}

void CodeGenerator::GenIfZ(Location *test, const char *label)
{
  code.push_back(new IfZ(test, label));
}

void CodeGenerator::GenGoto(const char *label)
{
  code.push_back(new Goto(label));
}

void CodeGenerator::GenReturn(Location *val)
{
  code.push_back(new Return(val));
}


BeginFunc *CodeGenerator::GenBeginFunc(FnDecl *fn)
{
  BeginFunc *result = new BeginFunc;
  code.push_back(insideFn = result);
  List<VarDecl*> *formals = fn->GetFormals();
  int start = OffsetToFirstParam;
  if (fn->IsMethodDecl()) start += VarSize;
  for (int i = 0; i < formals->NumElements(); i++)
    formals->Nth(i)->rtLoc = new Location(fpRelative, i*VarSize + start, formals->Nth(i)->GetName());
  curStackOffset = OffsetToFirstLocal;
  return result;
}

void CodeGenerator::GenEndFunc()
{
  code.push_back(new EndFunc());
  insideFn->SetFrameSize(OffsetToFirstLocal-curStackOffset);
  insideFn = NULL;
}

void CodeGenerator::GenPushParam(Location *param)
{
  code.push_back(new PushParam(param));
}

void CodeGenerator::GenPopParams(int numBytesOfParams)
{
  Assert(numBytesOfParams >= 0 && numBytesOfParams % VarSize == 0); // sanity check
  if (numBytesOfParams > 0)
    code.push_back(new PopParams(numBytesOfParams));
}

Location *CodeGenerator::GenLCall(const char *label, bool fnHasReturnValue)
{
  Location *result = fnHasReturnValue ? GenTempVar() : NULL;
  code.push_back(new LCall(label, result));
  return result;
}
  
Location *CodeGenerator::GenFunctionCall(const char *fnLabel, List<Location*> *args, bool hasReturnValue)
{
  for (int i = args->NumElements()-1; i >= 0; i--) // push params right to left
    GenPushParam(args->Nth(i));
  Location *result = GenLCall(fnLabel, hasReturnValue);
  GenPopParams(args->NumElements()*VarSize);
  return result;
}

Location *CodeGenerator::GenACall(Location *fnAddr, bool fnHasReturnValue)
{
  Location *result = fnHasReturnValue ? GenTempVar() : NULL;
  code.push_back(new ACall(fnAddr, result));
  return result;
}
  
Location *CodeGenerator::GenMethodCall(Location *rcvr,
			     Location *meth, List<Location*> *args, bool fnHasReturnValue)
{
  for (int i = args->NumElements()-1; i >= 0; i--)
    GenPushParam(args->Nth(i));
  GenPushParam(rcvr);	// hidden "this" parameter
  Location *result= GenACall(meth, fnHasReturnValue);
  GenPopParams((args->NumElements()+1)*VarSize);
  return result;
}
 
 
static struct _builtin {
  const char *label;
  int numArgs;
  bool hasReturn;
} builtins[] =
 {{"_Alloc", 1, true},
  {"_ReadLine", 0, true},
  {"_ReadInteger", 0, true},
  {"_StringEqual", 2, true},
  {"_PrintInt", 1, false},
  {"_PrintString", 1, false},
  {"_PrintBool", 1, false},
  {"_Halt", 0, false}};

Location *CodeGenerator::GenBuiltInCall(BuiltIn bn,Location *arg1, Location *arg2)
{
  Assert(bn >= 0 && bn < NumBuiltIns);
  struct _builtin *b = &builtins[bn];
  Location *result = NULL;

  if (b->hasReturn) result = GenTempVar();
                // verify appropriate number of non-NULL arguments given
  Assert((b->numArgs == 0 && !arg1 && !arg2)
	|| (b->numArgs == 1 && arg1 && !arg2)
	|| (b->numArgs == 2 && arg1 && arg2));
  if (arg2) code.push_back(new PushParam(arg2));
  if (arg1) code.push_back(new PushParam(arg1));
  code.push_back(new LCall(b->label, result));
  GenPopParams(VarSize*b->numArgs);
  return result;
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
   

 
