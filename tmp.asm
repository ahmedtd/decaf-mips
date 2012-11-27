# fndecl::emit: emitting function label
# fndecl::emit: generating Locations for formal parameters
# fndecl::emit: emitting code for body
# fndecl::emit: backpatching frame size to 4
# fndecl::emit: emitting end-of-function boilerplate
# fndecl::emit: done
	# standard Decaf preamble 
	  .text
	  .align 2
	  .globl main
#main: (label)
  main:
#BeginFunc 4
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 4	# decrement sp to make space for locals/temps
#