/* File: mips.cc
 * -------------
 * Implementation of Mips class, which is responsible for TAC->MIPS
 * translation, register allocation, etc.
 *
 * Julie Zelenski academic year 2001-02 for CS143
 * Loosely based on some earlier work by Steve Freund
 *
 * A simple final code generator to translate Tac to MIPS.
 * It uses simplistic algorithms, in particular, its register handling
 * and spilling strategy is inefficient to the point of begin mocked
 * by elementary school children.
 *
 * Dan Bentley, April 2002
 * A simpler final code generator to translate Tac to MIPS.
 * It uses algorithms without loops or conditionals, to make there be a
 * very clear and obvious translation between one and the other.
 * Specifically, it always loads operands off stacks, and stores the
 * result back.  This breaks bad code immediately, theoretically helping
 * students.
 */

#include "mips.h"
#include <cstdarg>
#include <cstring>

#include <iostream>
using std::cout;
using std::endl;
#include <stdexcept>
using std::logic_error;
#include <algorithm>
using std::make_heap;
using std::push_heap;
using std::pop_heap;
using std::iter_swap;
using std::find_if;
using std::begin;
using std::end;

// Helper to check if two variable locations are one and the same
// (same name, segment, and offset)
static bool LocationsAreSame(Location *var1, Location *var2)
{
   return (var1 == var2 ||
	     (var1 && var2
		&& !strcmp(var1->GetName(), var2->GetName())
		&& var1->GetSegment()  == var2->GetSegment()
		&& var1->GetOffset() == var2->GetOffset()));
}

Mips::~Mips()
{
    delete m_reg_alloc;
}

/* Method: SpillRegister
 * ---------------------
 * Used to spill a register from reg to dst.  All it does is emit a store
 * from that register to its location on the stack.
 */
void Mips::SpillRegister(Location *dst, Register reg)
{
  Assert(dst);
  const char *offsetFromWhere = dst->GetSegment() == fpRelative? regs[fp].name : regs[gp].name;
  Assert(dst->GetOffset() % 4 == 0); // all variables are 4 bytes in size
  Emit("sw %s, %d(%s)\t# spill %s from %s to %s%+d", regs[reg].name,
       dst->GetOffset(), offsetFromWhere, dst->GetName(), regs[reg].name,
       offsetFromWhere,dst->GetOffset());
}

/* Method: FillRegister
 * --------------------
 * Fill a register from location src into reg.
 * Simply load a word into a register.
 */
void Mips::FillRegister(Location *src, Register reg)
{
  Assert(src);
  const char *offsetFromWhere = src->GetSegment() == fpRelative? regs[fp].name : regs[gp].name;
  Assert(src->GetOffset() % 4 == 0); // all variables are 4 bytes in size
  Emit("lw %s, %d(%s)\t# fill %s to %s from %s%+d", regs[reg].name,
       src->GetOffset(), offsetFromWhere, src->GetName(), regs[reg].name,
       offsetFromWhere,src->GetOffset());
}



/* Method: Emit
 * ------------
 * General purpose helper used to emit assembly instructions in
 * a reasonable tidy manner.  Takes printf-style formatting strings
 * and variable arguments.
 */
void Mips::Emit(const char *fmt, ...)
{
  va_list args;
  char buf[1024];
  
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  if (buf[strlen(buf) - 1] != ':') printf("\t"); // don't tab in labels
  if (buf[0] != '#') printf("  ");   // outdent comments a little
  printf("%s", buf);
  if (buf[strlen(buf)-1] != '\n') printf("\n"); // end with a newline
}



/* Method: EmitLoadConstant
 * ------------------------
 * Used to assign variable an integer constant value.  Slaves dst into
 * a register (using GetRegister above) and then emits an li (load
 * immediate) instruction with the constant value.
 */
void Mips::EmitLoadConstant(Location *dst, int val)
{
    // Register reg = rd;
    
    // Get a register from the allocator
    Register dst_reg = m_reg_alloc->alloc_reg(dst, false);
    
    Emit("li %s, %d\t\t# load constant value %d into %s", regs[dst_reg].name,
         val, val, regs[dst_reg].name);

    // Free the dst reg, marking as dirty
    m_reg_alloc->free_reg(dst_reg, true);

    // SpillRegister(dst, rd);
}

/* Method: EmitLoadStringConstant
 * ------------------------------
 * Used to assign a variable a pointer to string constant. Emits
 * assembly directives to create a new null-terminated string in the
 * data segment and assigns it a unique label. Slaves dst into a register
 * and loads that label address into the register.
 */
void Mips::EmitLoadStringConstant(Location *dst, const char *str)
{
    static int strNum = 1;
    char label[16];
    sprintf(label, "_string%d", strNum++);
    Emit(".data\t\t\t# create string constant marked with label");
    Emit("%s: .asciiz %s", label, str);
    Emit(".text");
    EmitLoadLabel(dst, label);
}


/* Method: EmitLoadLabel
 * ---------------------
 * Used to load a label (ie address in text/data segment) into a variable.
 * Slaves dst into a register and emits an la (load address) instruction
 */
void Mips::EmitLoadLabel(Location *dst, const char *label)
{
    // Request a destination register
    Register dst_reg = m_reg_alloc->alloc_reg(dst, false);

    Emit("la %s, %s\t# load label", regs[dst_reg].name, label);

    // Free the reg, noting that it is dirty
    m_reg_alloc->free_reg(dst_reg, true);

    //SpillRegister(dst, rd);
}
 

/* Method: EmitCopy
 * ----------------
 * Used to copy the value of one variable to another.  Slaves both
 * src and dst into registers and then emits a move instruction to
 * copy the contents from src to dst.
 */
void Mips::EmitCopy(Location *dst, Location *src)
{
    // Register 

    Register src_reg = m_reg_alloc->alloc_reg(src);
    Register dst_reg = m_reg_alloc->alloc_reg(dst, false);
    
    // Mips assembly move
    Emit("move %s, %s \t# copy", regs[dst_reg].name, regs[src_reg].name);

    m_reg_alloc->free_reg(src_reg, false);
    m_reg_alloc->free_reg(dst_reg, true);

    // FillRegister(src, rd);
    // SpillRegister(dst, rd);
}


/* Method: EmitLoad
 * ----------------
 * Used to assign dst the contents of memory at the address in reference,
 * potentially with some positive/negative offset (defaults to 0).
 * Slaves both ref and dst to registers, then emits a lw instruction
 * using constant-offset addressing mode y(rx) which accesses the address
 * at an offset of y bytes from the address currently contained in rx.
 */
void Mips::EmitLoad(Location *dst, Location *reference, int offset)
{
    // FillRegister(reference, rs);

    Register dst_reg = m_reg_alloc->alloc_reg(dst, false);
    Register ref_reg = m_reg_alloc->alloc_reg(reference);

    Emit("lw %s, %d(%s) \t# load with offset",
         regs[dst_reg].name,
         offset,
         regs[ref_reg].name);
    
    m_reg_alloc->free_reg(dst_reg, true);
    m_reg_alloc->free_reg(ref_reg, false);

    //SpillRegister(dst, rd);
}


/* Method: EmitStore
 * -----------------
 * Used to write value to  memory at the address in reference,
 * potentially with some positive/negative offset (defaults to 0).
 * Slaves both ref and dst to registers, then emits a sw instruction
 * using constant-offset addressing mode y(rx) which writes to the address
 * at an offset of y bytes from the address currently contained in rx.
 */
void Mips::EmitStore(Location *reference, Location *value, int offset)
{
    Register val_reg = m_reg_alloc->alloc_reg(value);
    Register ref_reg = m_reg_alloc->alloc_reg(reference);

    // FillRegister(value, rs);
    // FillRegister(reference, rd);
    
    Emit("sw %s, %d(%s) \t# store with offset",
         regs[val_reg].name, offset, regs[ref_reg].name);
    
    m_reg_alloc->free_reg(val_reg, false);
    m_reg_alloc->free_reg(ref_reg, false);
}


/* Method: EmitBinaryOp
 * --------------------
 * Used to perform a binary operation on 2 operands and store result
 * in dst. All binary forms for arithmetic, logical, relational, equality
 * use this method. Slaves both operands and dst to registers, then
 * emits the appropriate instruction by looking up the mips name
 * for the particular op code.
 */
void Mips::EmitBinaryOp(BinaryOp::OpCode code, Location *dst, 
				 Location *op1, Location *op2)
{
    Register dst_reg = m_reg_alloc->alloc_reg(dst, false);
    Register op1_reg = m_reg_alloc->alloc_reg(op1);
    Register op2_reg = m_reg_alloc->alloc_reg(op2);

    // FillRegister(op1, rs);
    // FillRegister(op2, rt);
    
    Emit("%s %s, %s, %s\t",
         NameForTac(code),
         regs[dst_reg].name,
         regs[op1_reg].name,
         regs[op2_reg].name);
    
    // SpillRegister(dst, rd);

    m_reg_alloc->free_reg(dst_reg, true);
    m_reg_alloc->free_reg(op1_reg, false);
    m_reg_alloc->free_reg(op2_reg, false);
}


/* Method: EmitLabel
 * -----------------
 * Used to emit label marker. Before a label, we spill all registers since
 * we can't be sure what the situation upon arriving at this label (ie
 * starts new basic block), and rather than try to be clever, we just
 * wipe the slate clean.
 */
void Mips::EmitLabel(const char *label)
{
    m_reg_alloc->spill_dirty();
    m_reg_alloc->reinit();

    Emit("%s:", label);
}


/* Method: EmitGoto
 * ----------------
 * Used for an unconditional transfer to a named label. Before a goto,
 * we spill all registers, since we don't know what the situation is
 * we are heading to (ie this ends current basic block) and rather than
 * try to be clever, we just wipe slate clean.
 */
void Mips::EmitGoto(const char *label)
{
    // A goto ends a basic block, so spill all dirty registers and reinitialize
    // the allocator
    m_reg_alloc->spill_dirty();
    m_reg_alloc->reinit();

    Emit("b %s\t\t# unconditional branch", label);
}


/* Method: EmitIfZ
 * ---------------
 * Used for a conditional branch based on value of test variable.
 * We slave test var to register and use in the emitted test instruction,
 * either beqz. See comments above on Goto for why we spill
 * all registers here.
 */
void Mips::EmitIfZ(Location *test, const char *label)
{
    // This instruction ends a basic block
    m_reg_alloc->spill_dirty();

    Register tst_reg = m_reg_alloc->alloc_reg(test);

    // FillRegister(test, rs);
    
    Emit("beqz %s, %s\t# branch if %s is zero ",
         regs[tst_reg].name,
         label,
         test->GetName());

    // The test register is not modified by the branch
    m_reg_alloc->free_reg(tst_reg, false);
    
    // After a branch, we should have the allocator reset its state
    m_reg_alloc->reinit();
}


/* Method: EmitParam
 * -----------------
 * Used to push a parameter on the stack in anticipation of upcoming
 * function call. Decrements the stack pointer by 4. Slaves argument into
 * register and then stores contents to location just made at end of
 * stack.
 */
void Mips::EmitParam(Location *arg)
{ 
    Emit("subu $sp, $sp, 4\t# decrement sp to make space for param");
    
    // FillRegister(arg, rs);
    
    Register arg_reg = m_reg_alloc->alloc_reg(arg);

    Emit("sw %s, 4($sp)\t# copy param value to stack", regs[arg_reg].name);

    m_reg_alloc->free_reg(arg_reg, false);
}


/* Method: EmitCallInstr
 * ---------------------
 * Used to effect a function call. All necessary arguments should have
 * already been pushed on the stack, this is the last step that
 * transfers control from caller to callee.  See comments on Goto method
 * above for why we spill all registers before making the jump. We issue
 * jal for a label, a jalr if address in register. Both will save the
 * return address in $ra. If there is an expected result passed, we slave
 * the var to a register and copy function return value from $v0 into that
 * register.  
 */
void Mips::EmitCallInstr(Location *result, const char *fn, bool isLabel)
{
    m_reg_alloc->spill_dirty();

    Emit("%s %-15s\t# jump to function",
         isLabel? "jal": "jalr",
         fn);

    m_reg_alloc->reinit();
    
    if (result != NULL)
    {
        Register result_reg = m_reg_alloc->alloc_reg(result);

        Emit("move %s, %s\t\t# copy function return value from $v0",
             regs[result_reg].name,
             regs[v0].name);
        
        // SpillRegister(result, rd);

        m_reg_alloc->free_reg(result_reg, true);
    }
}


// Two covers for the above method for specific LCall/ACall variants
void Mips::EmitLCall(Location *result, const char *label)
{ 
    // EmitCallInstr(dst, label, true);

    m_reg_alloc->spill_dirty();

    Emit("%s %-15s\t# jump to function",
         "jal",
         label);
    
    m_reg_alloc->reinit();
    
    if (result != NULL)
    {
        Register result_reg = m_reg_alloc->alloc_reg(result, false);

        Emit("move %s, %s\t\t# copy function return value from $v0",
             regs[result_reg].name,
             regs[v0].name);
        
        // SpillRegister(result, rd);

        m_reg_alloc->free_reg(result_reg, true);
    }
}

void Mips::EmitACall(Location *result, Location *fn)
{
    // FillRegister(fn, rs);
    // EmitCallInstr(dst, regs[rs].name, false);

    m_reg_alloc->spill_dirty();

    Register function_reg = m_reg_alloc->alloc_reg(fn);
   
    Emit("%s %-15s\t# jump to function",
         "jalr",
         regs[function_reg].name);

    m_reg_alloc->free_reg(function_reg, false);
    
    m_reg_alloc->reinit();

    if (result != NULL)
    {
        Register result_reg = m_reg_alloc->alloc_reg(result, false);

        Emit("move %s, %s\t\t# copy function return value from $v0",
             regs[result_reg].name,
             regs[v0].name);
        
        // SpillRegister(result, rd);

        m_reg_alloc->free_reg(result_reg, true);
    }
}

/*
 * We remove all parameters from the stack after a completed call
 * by adjusting the stack pointer upwards.
 */
void Mips::EmitPopParams(int bytes)
{
    if (bytes != 0)
        Emit("add $sp, $sp, %d\t# pop params off stack", bytes);
}


/* Method: EmitReturn
 * ------------------
 * Used to emit code for returning from a function (either from an
 * explicit return or falling off the end of the function body).
 * If there is an expression to return, we slave that variable into
 * a register and move its contents to $v0 (the standard register for
 * function result).  Before exiting, we spill dirty registers (to
 * commit contents of slaved registers to memory, necessary for
 * consistency, see comments at SpillForEndFunction above). We also
 * do the last part of the callee's job in function call protocol,
 * which is to remove our locals/temps from the stack, remove
 * saved registers ($fp and $ra) and restore previous values of
 * $fp and $ra so everything is returned to the state we entered.
 * We then emit jr to jump to the saved $ra.
 */
void Mips::EmitReturn(Location *return_loc)
{ 
    if (return_loc != NULL) 
    {
        const char *offsetFromWhere = return_loc->GetSegment() == fpRelative
            ? regs[fp].name
            : regs[gp].name;

        int offset = return_loc->GetOffset();

        // Check if the return value is in a register
        if(m_reg_alloc->location_shadowed(return_loc))
        {
            // If so, get the register
            Register retval_loc = m_reg_alloc->alloc_reg(return_loc, false);
            
            Emit("move $v0, %s",
                 regs[retval_loc].name);
        }
        else
        {
            // For return, load directly to v0
            Emit("lw $v0, %d(%s)\t# fill $v0 from %d(%s)",
                 offset,
                 offsetFromWhere,
                 offset,
                 offsetFromWhere);
        }     
    }
    
    // End the basic block, but don't spill registers, because who cares?
    // m_reg_alloc->spill_dirty();
    m_reg_alloc->reinit();

    Emit("move $sp, $fp\t\t# pop callee frame off stack");
    Emit("lw $ra, -4($fp)\t# restore saved ra");
    Emit("lw $fp, 0($fp)\t# restore saved fp");
    Emit("jr $ra\t\t# return from function");
}


/* Method: EmitBeginFunction
 * -------------------------
 * Used to handle the callee's part of the function call protocol
 * upon entering a new function. We decrement the $sp to make space
 * and then save the current values of $fp and $ra (since we are
 * going to change them), then set up the $fp and bump the $sp down
 * to make space for all our locals/temps.
 */
void Mips::EmitBeginFunction(int stackFrameSize)
{
  Assert(stackFrameSize >= 0);
  Emit("subu $sp, $sp, 8\t# decrement sp to make space to save ra, fp");
  Emit("sw $fp, 8($sp)\t# save fp");
  Emit("sw $ra, 4($sp)\t# save ra");
  Emit("addiu $fp, $sp, 8\t# set up new fp");

  if (stackFrameSize != 0)
    Emit("subu $sp, $sp, %d\t# decrement sp to make space for locals/temps",
	   stackFrameSize);
}


/* Method: EmitEndFunction
 * -----------------------
 * Used to end the body of a function. Does an implicit return in fall off
 * case to clean up stack frame, return to caller etc. See comments on
 * EmitReturn above.
 */
void Mips::EmitEndFunction()
{ 
  Emit("# (below handles reaching end of fn body with no explicit return)");
  EmitReturn(NULL);
}



/* Method: EmitVTable
 * ------------------
 * Used to layout a vtable. Uses assembly directives to set up new
 * entry in data segment, emits label, and lays out the function
 * labels one after another.
 */
void Mips::EmitVTable(const char *label, List<const char*> *methodLabels)
{
  Emit(".data");
  Emit(".align 2");
  Emit("%s:\t\t# label for class %s vtable", label, label);
  for (int i = 0; i < methodLabels->NumElements(); i++)
    Emit(".word %s\n", methodLabels->Nth(i));
  Emit(".text");
}


/* Method: EmitPreamble
 * --------------------
 * Used to emit the starting sequence needed for a program. Not much
 * here, but need to indicate what follows is in text segment and
 * needs to be aligned on word boundary. main is our only global symbol.
 */
void Mips::EmitPreamble()
{
  Emit("# standard Decaf preamble ");
  Emit(".text");
  Emit(".align 2");
  Emit(".globl main");
}


/* Method: NameForTac
 * ------------------
 * Returns the appropriate MIPS instruction (add, seq, etc.) for
 * a given BinaryOp:OpCode (BinaryOp::Add, BinaryOp:Equals, etc.). 
 * Asserts if asked for name of an unset/out of bounds code.
 */
const char *Mips::NameForTac(BinaryOp::OpCode code)
{
  Assert(code >=0 && code < BinaryOp::NumOps);
  const char *name = mipsName[code];
  Assert(name != NULL);
  return name;
}

/* Constructor
 * ----------
 * Constructor sets up the mips names and register descriptors to
 * the initial starting state.
 */
Mips::Mips()
{
    // Fill some mips instruction names
    mipsName[BinaryOp::Add] = "add";
    mipsName[BinaryOp::Sub] = "sub";
    mipsName[BinaryOp::Mul] = "mul";
    mipsName[BinaryOp::Div] = "div";
    mipsName[BinaryOp::Mod] = "rem";
    mipsName[BinaryOp::Eq] = "seq";
    mipsName[BinaryOp::Less] = "slt";
    mipsName[BinaryOp::And] = "and";
    mipsName[BinaryOp::Or] = "or";

    // Fill out some register information
    regs[zero] = (RegContents){false, NULL, "$zero", false};
    regs[at] = (RegContents){false, NULL, "$at", false};
    regs[v0] = (RegContents){false, NULL, "$v0", false};
    regs[v1] = (RegContents){false, NULL, "$v1", false};
    regs[a0] = (RegContents){false, NULL, "$a0", false};
    regs[a1] = (RegContents){false, NULL, "$a1", false};
    regs[a2] = (RegContents){false, NULL, "$a2", false};
    regs[a3] = (RegContents){false, NULL, "$a3", false};
    regs[k0] = (RegContents){false, NULL, "$k0", false};
    regs[k1] = (RegContents){false, NULL, "$k1", false};
    regs[gp] = (RegContents){false, NULL, "$gp", false};
    regs[sp] = (RegContents){false, NULL, "$sp", false};
    regs[fp] = (RegContents){false, NULL, "$fp", false};
    regs[ra] = (RegContents){false, NULL, "$ra", false};
    regs[t0] = (RegContents){false, NULL, "$t0", true};
    regs[t1] = (RegContents){false, NULL, "$t1", true};
    regs[t2] = (RegContents){false, NULL, "$t2", true};
    regs[t3] = (RegContents){false, NULL, "$t3", true};
    regs[t4] = (RegContents){false, NULL, "$t4", true};
    regs[t5] = (RegContents){false, NULL, "$t5", true};
    regs[t6] = (RegContents){false, NULL, "$t6", true};
    regs[t7] = (RegContents){false, NULL, "$t7", true};
    regs[t8] = (RegContents){false, NULL, "$t8", true};
    regs[t9] = (RegContents){false, NULL, "$t9", true};
    regs[s0] = (RegContents){false, NULL, "$s0", true};
    regs[s1] = (RegContents){false, NULL, "$s1", true};
    regs[s2] = (RegContents){false, NULL, "$s2", true};
    regs[s3] = (RegContents){false, NULL, "$s3", true};
    regs[s4] = (RegContents){false, NULL, "$s4", true};
    regs[s5] = (RegContents){false, NULL, "$s5", true};
    regs[s6] = (RegContents){false, NULL, "$s6", true};
    regs[s7] = (RegContents){false, NULL, "$s7", true};

    // Assign the fixed operation registers
    // rs = t0; rt = t1; rd = t2;

    // Get an LRU allocator
    m_reg_alloc = new LRUAllocator(this);
}

const char *Mips::mipsName[BinaryOp::NumOps];

Mips::RegisterAllocator::~RegisterAllocator()
{
}

Mips::LRUAllocator::LRUAllocator(Mips *mips)
    : m_time(0),
      m_mips(mips)
{
    // Regs t0 - t9
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t0, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t1, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t2, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t3, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t4, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t5, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t6, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t7, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t8, nullptr, 0);
    m_reg_heap.emplace_back(reg_record::reg_status::clean, -1, t9, nullptr, 0);

    // Format the register heap
    make_heap(begin(m_reg_heap), end(m_reg_heap));

    m_reg_heap_end = end(m_reg_heap);
}

Mips::LRUAllocator::~LRUAllocator()
{
}

Mips::Register Mips::LRUAllocator::alloc_reg(Location *shadowed, bool load)
{
    cout << "# alloc_reg(); called for " << shadowed->GetName();

    // Is this location already shadowed in a register?
    auto search_record = find_if(
        begin(m_reg_heap),
        end(m_reg_heap),
        [&](const reg_record &rec) { return (rec.shadowed == shadowed); }
    );

    // If the location is already shadowed, return the register that shadows it
    if(search_record != end(m_reg_heap))
    {
        // Update the access time of the register
        (*search_record).time = m_time++;
        (*search_record).alloc_count++;
       
        Register retval = (*search_record).reg;

        // If this is the first time allocating this register for this
        // instruction, then we need to pop it out of the heap
        --m_reg_heap_end;
        iter_swap(search_record, m_reg_heap_end);

        // Restore the heap properties
        make_heap(begin(m_reg_heap), m_reg_heap_end);

        cout << ", already in register " << m_mips->regs[retval].name << endl;

        return retval;
    }

    // Otherwise, select the most suitable register candidate
    // (the one at the top of the heap).

    // Sanity checks
    if(m_reg_heap_end <= begin(m_reg_heap))
        throw logic_error(
            "Mips::LRUAllocator::alloc_reg(): Register allocation limit hit.");

    if(m_reg_heap[0].alloc_count > 0)
        throw logic_error(
            "Mips::LRUAllocator::alloc_reg(): Allocation candidate marked as "
            "already allocated.");

    Register retval = m_reg_heap[0].reg;

    cout << ", assigning reg " << m_mips->regs[retval].name;
    
    // Spill the register if necessary
    if(m_reg_heap[0].status == reg_record::reg_status::dirty)
    {
        cout << ", reg is dirty, spilling" << endl;
        
        // Check to make sure the register is shadowing a location
        if(m_reg_heap[0].shadowed == nullptr)
            throw logic_error(
                "Mips::LRUAllocator::alloc_reg(): Allocation candidate marked"
                " as dirty but has no shadowing register.");

        
        // Emit a spill to the register's shadowed location
        m_mips->SpillRegister(m_reg_heap[0].shadowed, m_reg_heap[0].reg);

        // Now the register is clean and no longer shadowing a location
    }
    else
    {
        cout << endl;
    }

    // Pop the heap
    m_reg_heap[0].status = reg_record::reg_status::clean;
    m_reg_heap[0].alloc_count++;
    m_reg_heap[0].shadowed = shadowed;
    m_reg_heap[0].time = m_time++;
    pop_heap(begin(m_reg_heap), m_reg_heap_end);
    m_reg_heap_end--;

    // Fill the register from the location
    if(load)
        m_mips->FillRegister(shadowed, retval);

    

    return retval;
}

void Mips::LRUAllocator::free_reg(Mips::Register to_free, bool is_dirty)
{
    cout << "# Free" << endl;

    // Sanity checks
    if(m_reg_heap_end >= end(m_reg_heap))
        throw logic_error(
            "Mips::LRUAllocator::free_reg(): attempted to free a register when"
            " there are no registers allocated.");

    // Find the reg_record for to_free
    auto free_record = find_if(
        begin(m_reg_heap),
        end(m_reg_heap),
        [&](const reg_record &rec) { return rec.reg == to_free; }
    );

    // Make sure the record exists
    if(free_record == end(m_reg_heap))
        throw logic_error(
            "Mips::LRUAllocator::free_reg(): Attempted to free a reg that is"
            " not begin trcaked by the allocator.  If this reg was obtained"
            " from alloc_reg(), this is a serious error.");

    // Make sure that this record is currently allocated
    if((*free_record).alloc_count == 0)
        throw logic_error(
            "Mips::LRUAllocator::free_reg(): attempted to free a reg that is"
            " not currently allocated.");

    // Update the record for the reg we are about to free

    // Dirty status is sticky.  If the location was dirty before this free (due
    // to aliasing of registers), then it should remain dirty after this free.
    (*free_record).status =
        ((*free_record).status == reg_record::reg_status::dirty)
        ? reg_record::reg_status::dirty
        : is_dirty
        ? reg_record::reg_status::dirty 
        : reg_record::reg_status::clean;

    // Update the last access time of this register
    (*free_record).time = m_time++;

    // Update the allocation count of this register
    (*free_record).alloc_count--;

    // Move the record corresponding to to_free to the end of the heap zone
    iter_swap(m_reg_heap_end, free_record);

    // Now m_reg_heap_end points to the record in question

    // Make room on the heap for our pending insertion
    m_reg_heap_end++;

    // Perform the heap insertion
    push_heap(begin(m_reg_heap), m_reg_heap_end);
    
}

bool Mips::LRUAllocator::location_shadowed(Location *loc)
{
    // Is this location already shadowed in a register?
    auto search_record = find_if(
        begin(m_reg_heap),
        end(m_reg_heap),
        [&](const reg_record &rec) { return (rec.shadowed == loc); }
    );

    // If the location is already shadowed, return the register that shadows it
    return search_record != end(m_reg_heap);
}

void Mips::LRUAllocator::reinit()
{
    for(reg_record &record : m_reg_heap)
    {
        record.status = reg_record::reg_status::clean;
        record.time = -1;
        record.shadowed = nullptr;
        record.alloc_count = 0;
    }

    m_time = 0;
    m_reg_heap_end = end(m_reg_heap);

    make_heap(begin(m_reg_heap), end(m_reg_heap));

    m_reg_heap_end = end(m_reg_heap);
}

void Mips::LRUAllocator::spill_dirty()
{
    for(reg_record &record : m_reg_heap)
    {
        if(record.alloc_count > 0)
            throw logic_error(
                "Mips::LRUAllocator::spill_dirty(): Called while there are"
                " still extant register allocations.");

        if(record.status == reg_record::reg_status::dirty)
            if(record.shadowed == nullptr)
                throw logic_error(
                    "Mips::LRUAllocator::spill_dirty(): record marked as dirty,"
                    " but has no shadowed location to spill to.");
    }
    for(reg_record &record : m_reg_heap)
    {
        if(record.status == reg_record::reg_status::dirty)
        {
            // If the reg is dirty, spill it

            m_mips->SpillRegister(record.shadowed, record.reg);

            record.status = reg_record::reg_status::clean;
            record.time = m_time++;

            // Don't set shadowed to nullptr, as branches and others of their
            // ilk will still use the fact the a location is shadowed.
        }
    }

    // Reform the heap to reflect the changes
    make_heap(begin(m_reg_heap), end(m_reg_heap));
}

Mips::LRUAllocator::reg_record::reg_record(
    reg_status in_status,
    int in_time,
    Register in_reg,
    Location *in_shadowed,
    size_t in_alloc_count)
    : status(in_status),
      time(in_time),
      reg(in_reg),
      shadowed(in_shadowed),
      alloc_count(in_alloc_count)
{
}

void Mips::LRUAllocator::reg_record::operator=(const reg_record &right)
{
    status = right.status;
    time = right.time;
    reg = right.reg;
    shadowed = right.shadowed;
    alloc_count = right.alloc_count;
}

bool Mips::LRUAllocator::reg_record::operator==(const reg_record &right) const
{
    return (status == right.status)
        && (time == right.time)
        && (reg == right.reg)
        && (shadowed == right.shadowed)
        && (alloc_count == right.alloc_count);
}

bool Mips::LRUAllocator::reg_record::operator<(const reg_record &right) const
{
    if(alloc_count > 0)
    {
        // An allocated register always comes later than a non-allocated
        // register.
        return true;
    }
    else if(
        (status == reg_status::clean && right.status == reg_status::clean)
        ||(status == reg_status::dirty && right.status == reg_status::dirty)
    )
    {
        return right.time < time;
    }
    else if(status == reg_status::clean && right.status == reg_status::dirty)
    {
        return false;
    }
    else
    {
        // If this is dirty and right is clean
        return true;
    }

    throw logic_error("Mips::LRUAllocator::reg_record::operator<():"
                      " unreachable code.");
    
    return false;
}


