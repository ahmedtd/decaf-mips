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

#include <vector>
using std::vector;

#include "tac.h"
#include "list.h"
class Location;


class Mips
{
private:
    typedef enum {
        zero, // Always 0
        at,   // Reserved for assembler
        v0,   // Return values for functions and expressions
        v1,   
        a0,   // Arguments (not preserved across calls)
        a1,
        a2,
        a3,
        s0,   // Callee save ragisters
        s1,
        s2,
        s3,
        s4,
        s5,
        s6,
        s7,
        t0,   // Caller save registers
        t1,
        t2,
        t3,
        t4, 
        t5,
        t6,
        t7,
        t8,
        t9,
        k0,   // Reserved for interrupt handler
        k1,
        gp,   // Pointer to global segment
        sp,   // Stack pointer
        fp,   // Frame pointer
        ra,   // Return address
        NumRegs
    } Register;

    // Record of the "current" contents of a register
    struct RegContents
    {
        bool isDirty;
        Location *var;
        const char *name;
        bool isGeneralPurpose;
    } regs[NumRegs];

    // Fixed registers for operations.  Removed because we need dynamic register
    // allocation.
    // Register rs;
    // Register rt;
    // Register rd;

    typedef enum {
        ForRead,
        ForWrite
    } Reason;
    
    // Load from source location to register
    void FillRegister(Location *src, Register reg);

    // Store from register to destination location
    void SpillRegister(Location *dst, Register reg);

    
    void EmitCallInstr(Location *dst, const char *fn, bool isL);
    

    static const char *mipsName[BinaryOp::NumOps];

    static const char *NameForTac(BinaryOp::OpCode code);

    Instruction* currentInstruction;

    // Register allocation classes
    class RegisterAllocator;    
    class LRUAllocator;

    RegisterAllocator *m_reg_alloc;
    
 public:
    
    Mips();
    virtual ~Mips();

    static void Emit(const char *fmt, ...);
    
    void EmitLoadConstant(Location *dst, int val);
    void EmitLoadStringConstant(Location *dst, const char *str);
    void EmitLoadLabel(Location *dst, const char *label);

    void EmitLoad(Location *dst, Location *reference, int offset);
    void EmitStore(Location *reference, Location *value, int offset);
    void EmitCopy(Location *dst, Location *src);

    void EmitBinaryOp(BinaryOp::OpCode code, Location *dst, 
			    Location *op1, Location *op2);

    void EmitLabel(const char *label);
    void EmitGoto(const char *label);
    void EmitIfZ(Location *test, const char*label);
    void EmitReturn(Location *returnVal);

    void EmitBeginFunction(int frameSize);
    void EmitEndFunction();

    void EmitParam(Location *arg);
    void EmitLCall(Location *result, const char* label);
    void EmitACall(Location *result, Location *fnAddr);
    void EmitPopParams(int bytes);

    void EmitVTable(const char *label, List<const char*> *methodLabels);

    void EmitPreamble();

    class CurrentInstruction;
};

// Adds CurrentInstruction to the Mips object
class Mips::CurrentInstruction
{
public:
  CurrentInstruction(Mips& mips, Instruction* instr)
    : mips( mips )
  {
    mips.currentInstruction= instr;
  }

  ~CurrentInstruction()
  {
    mips.currentInstruction= NULL;
  }

private:
  Mips& mips;
};

class Mips::RegisterAllocator
{
public:
    virtual ~RegisterAllocator() = 0;

    // Request a register for use in the current instruction.
    virtual Register alloc_reg(Location *shadowed, bool load = true) = 0;

    // Return a register to the allocator after use in the current
    // instruction. The register can be marked as dirty if necessary.
    virtual void free_reg(Register to_free, bool is_dirty) = 0;

    virtual void reinit() = 0;
    virtual void spill_dirty() = 0;
    
    virtual bool location_shadowed(Location *loc) = 0;
};

class Mips::LRUAllocator
    : public Mips::RegisterAllocator
{
public:
    LRUAllocator(Mips *mips);
    virtual ~LRUAllocator();

    virtual Register alloc_reg(Location *shadowed, bool load = true);
    virtual void free_reg(Register to_free, bool is_dirty);

    virtual void reinit();
    virtual void spill_dirty();

    virtual bool location_shadowed(Location *loc);

private:
    struct reg_record
    {
        enum class reg_status { dirty, clean };
        reg_status status;
        int time;
        Register reg;
        Location *shadowed;
        size_t alloc_count;

        reg_record(reg_status in_reg_status,
                   int in_time,
                   Register in_reg,
                   Location *in_shadowed,
                   size_t in_alloc_count);

        void operator=(const reg_record &right);

        bool operator==(const reg_record &right) const;

        bool operator<(const reg_record &right) const;
    };
    
    vector<reg_record>::iterator m_reg_heap_end;
    vector<reg_record> m_reg_heap;

    int m_time;

    Mips *m_mips;
};


#endif
 
