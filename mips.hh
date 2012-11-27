/* File: mips.h
 * ------------
 * The Mips class defines an object capable of emitting MIPS
 * instructions and managing the allocation and use of registers.
 * It is used by the Tac instruction classes to convert each
 * instruction to the appropriate MIPS equivalent.
 *
 * You can scan this code to see it works and get a sense of what
 * it does.  You will not need to modify this class unless
 * you're attempting some machine-specific optimizations. 
 *
 * It comments the emitted assembly but the commenting for the code
 * in the class itself is pretty sparse. The SPIM manual (see link
 * from other materials on our web site) has more detailed documentation
 * on the MIPS architecture, instruction set, calling conventions, etc.
 */

#ifndef _H_mips
#define _H_mips

#include "tac.hh"

class Location;

class Mips {
  private:
    typedef enum {zero, at, v0, v1, a0, a1, a2, a3,
			t0, t1, t2, t3, t4, t5, t6, t7,
			s0, s1, s2, s3, s4, s5, s6, s7,
			t8, t9, k0, k1, gp, sp, fp, ra, NumRegs } Register;

    struct RegContents {
	bool isDirty;
	Location *var;
	const char *name;
	bool isGeneralPurpose;
    } regs[NumRegs];

    Register rs, rt, rd;

    typedef enum { ForRead, ForWrite } Reason;
    
    void FillRegister(const Location &src, Register reg);
    void SpillRegister(const Location &dst, Register reg);

    void EmitCallInstr(const Location &dst, const string &fn, bool isL);
    void EmitCallInstr(const string &fn, const bool isLabel);
    
    static const char *mipsName[BinaryOp::NumOps];
    static const char *NameForTac(BinaryOp::OpCode code);

 public:
    
    Mips();

    static void Emit(const char* fmt, ...);
    
    void EmitLoadConstant(const Location &dst, int val);
    void EmitLoadStringConstant(const Location &dst, const string &str);
    void EmitLoadLabel(const Location &dst, const string &label);

    void EmitLoad(const Location &dst, const Location &reference, int offset);
    void EmitStore(const Location &reference, const Location &value, int offset);
    void EmitCopy(const Location &dst, const Location &src);

    void EmitBinaryOp(BinaryOp::OpCode code, const Location &dst, 
			    const Location &op1, const Location &op2);

    void EmitLabel(const string &label);
    void EmitGoto(const string &label);
    void EmitIfZ(const Location &test, const string &label);
    void EmitReturn();
    void EmitReturn(const Location &returnVal);

    void EmitBeginFunction(int frameSize);
    void EmitEndFunction();

    void EmitParam(const Location &arg);
    void EmitLCall(const Location &result, const string &label);
    void EmitLCall(const string &label);
    void EmitACall(const Location &result, const Location &fnAddr);
    void EmitACall(const Location &fnAddr);
    void EmitPopParams(int bytes);

    void EmitVTable(const string &label, const vector<string> &methodLabels);

    void EmitPreamble();
};


#endif
 
