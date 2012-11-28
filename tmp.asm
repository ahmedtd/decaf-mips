	# standard Decaf preamble 
	  .text
	  .align 2
	  .globl main
  _Wild:
	# BeginFunc 80
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 80	# decrement sp to make space for locals/temps
	# _tmp0 = 0
# alloc_reg(); called for _tmp0, assigning reg $t6
	  li $t6, 0		# load constant value 0 into $t6
# Free
	# i = _tmp0
# alloc_reg(); called for _tmp0, already in register $t6
# alloc_reg(); called for i, assigning reg $t1
	  move $t1, $t6 	# copy
# Free
# Free
	  sw $t6, -12($fp)	# spill _tmp0 from $t6 to $fp-12
	  sw $t1, -8($fp)	# spill i from $t1 to $fp-8
  _L0:
	# _tmp1 = *(names + -4)
# alloc_reg(); called for _tmp1, assigning reg $t0
# alloc_reg(); called for names, assigning reg $t2
	  lw $t2, 4($fp)	# fill names to $t2 from $fp+4
	  lw $t0, -4($t2) 	# load with offset
# Free
# Free
	# _tmp2 = i < _tmp1
# alloc_reg(); called for _tmp2, assigning reg $t8
# alloc_reg(); called for i, assigning reg $t9
	  lw $t9, -8($fp)	# fill i to $t9 from $fp-8
# alloc_reg(); called for _tmp1, already in register $t0
	  slt $t8, $t9, $t0	
# Free
# Free
# Free
	# IfZ _tmp2 Goto _L1
	  sw $t8, -20($fp)	# spill _tmp2 from $t8 to $fp-20
	  sw $t0, -16($fp)	# spill _tmp1 from $t0 to $fp-16
# alloc_reg(); called for _tmp2, already in register $t8
	  beqz $t8, _L1	# branch if _tmp2 is zero 
# Free
	# _tmp3 = 0
# alloc_reg(); called for _tmp3, assigning reg $t2
	  li $t2, 0		# load constant value 0 into $t2
# Free
	# _tmp4 = i < _tmp3
# alloc_reg(); called for _tmp4, assigning reg $t5
# alloc_reg(); called for i, assigning reg $t3
	  lw $t3, -8($fp)	# fill i to $t3 from $fp-8
# alloc_reg(); called for _tmp3, already in register $t2
	  slt $t5, $t3, $t2	
# Free
# Free
# Free
	# _tmp5 = *(names + -4)
# alloc_reg(); called for _tmp5, assigning reg $t0
# alloc_reg(); called for names, assigning reg $t7
	  lw $t7, 4($fp)	# fill names to $t7 from $fp+4
	  lw $t0, -4($t7) 	# load with offset
# Free
# Free
	# _tmp6 = i < _tmp5
# alloc_reg(); called for _tmp6, assigning reg $t4
# alloc_reg(); called for i, already in register $t3
# alloc_reg(); called for _tmp5, already in register $t0
	  slt $t4, $t3, $t0	
# Free
# Free
# Free
	# _tmp7 = _tmp6 == _tmp3
# alloc_reg(); called for _tmp7, assigning reg $t6
# alloc_reg(); called for _tmp6, already in register $t4
# alloc_reg(); called for _tmp3, already in register $t2
	  seq $t6, $t4, $t2	
# Free
# Free
# Free
	# _tmp8 = _tmp4 || _tmp7
# alloc_reg(); called for _tmp8, assigning reg $t1
# alloc_reg(); called for _tmp4, already in register $t5
# alloc_reg(); called for _tmp7, already in register $t6
	  or $t1, $t5, $t6	
# Free
# Free
# Free
	# IfZ _tmp8 Goto _L2
	  sw $t2, -24($fp)	# spill _tmp3 from $t2 to $fp-24
	  sw $t0, -32($fp)	# spill _tmp5 from $t0 to $fp-32
	  sw $t4, -36($fp)	# spill _tmp6 from $t4 to $fp-36
	  sw $t1, -44($fp)	# spill _tmp8 from $t1 to $fp-44
	  sw $t5, -28($fp)	# spill _tmp4 from $t5 to $fp-28
	  sw $t6, -40($fp)	# spill _tmp7 from $t6 to $fp-40
# alloc_reg(); called for _tmp8, already in register $t1
	  beqz $t1, _L2	# branch if _tmp8 is zero 
# Free
	# _tmp9 = "Decaf runtime error: Array subscript out of bound..."
	  .data			# create string constant marked with label
	  _string1: .asciiz "Decaf runtime error: Array subscript out of bounds\n"
	  .text
# alloc_reg(); called for _tmp9, assigning reg $t3
	  la $t3, _string1	# load label
# Free
	# PushParam _tmp9
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp9, already in register $t3
	  sw $t3, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t3, -48($fp)	# spill _tmp9 from $t3 to $fp-48
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# LCall _Halt
	  jal _Halt          	# jump to function
  _L2:
	# _tmp10 = 4
# alloc_reg(); called for _tmp10, assigning reg $t9
	  li $t9, 4		# load constant value 4 into $t9
# Free
	# _tmp11 = _tmp10 * i
# alloc_reg(); called for _tmp11, assigning reg $t8
# alloc_reg(); called for _tmp10, already in register $t9
# alloc_reg(); called for i, assigning reg $t5
	  lw $t5, -8($fp)	# fill i to $t5 from $fp-8
	  mul $t8, $t9, $t5	
# Free
# Free
# Free
	# _tmp12 = names + _tmp11
# alloc_reg(); called for _tmp12, assigning reg $t1
# alloc_reg(); called for names, assigning reg $t7
	  lw $t7, 4($fp)	# fill names to $t7 from $fp+4
# alloc_reg(); called for _tmp11, already in register $t8
	  add $t1, $t7, $t8	
# Free
# Free
# Free
	# _tmp13 = *(_tmp12)
# alloc_reg(); called for _tmp13, assigning reg $t4
# alloc_reg(); called for _tmp12, already in register $t1
	  lw $t4, 0($t1) 	# load with offset
# Free
# Free
	# PushParam answer
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for answer, assigning reg $t0
	  lw $t0, 8($fp)	# fill answer to $t0 from $fp+8
	  sw $t0, 4($sp)	# copy param value to stack
# Free
	# PushParam _tmp13
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp13, already in register $t4
	  sw $t4, 4($sp)	# copy param value to stack
# Free
	# _tmp14 = LCall _StringEqual
	  sw $t9, -52($fp)	# spill _tmp10 from $t9 to $fp-52
	  sw $t8, -56($fp)	# spill _tmp11 from $t8 to $fp-56
	  sw $t1, -60($fp)	# spill _tmp12 from $t1 to $fp-60
	  sw $t4, -64($fp)	# spill _tmp13 from $t4 to $fp-64
	  jal _StringEqual   	# jump to function
# alloc_reg(); called for _tmp14, assigning reg $t8
	  move $t8, $v0		# copy function return value from $v0
# Free
	# PopParams 8
	  add $sp, $sp, 8	# pop params off stack
	# IfZ _tmp14 Goto _L3
	  sw $t8, -68($fp)	# spill _tmp14 from $t8 to $fp-68
# alloc_reg(); called for _tmp14, already in register $t8
	  beqz $t8, _L3	# branch if _tmp14 is zero 
# Free
	# _tmp15 = 1
# alloc_reg(); called for _tmp15, assigning reg $t2
	  li $t2, 1		# load constant value 1 into $t2
# Free
	# Return _tmp15
# alloc_reg(); called for _tmp15, already in register $t2
	  move $v0, $t2
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _L3:
	# _tmp16 = 1
# alloc_reg(); called for _tmp16, assigning reg $t3
	  li $t3, 1		# load constant value 1 into $t3
# Free
	# _tmp17 = i + _tmp16
# alloc_reg(); called for _tmp17, assigning reg $t5
# alloc_reg(); called for i, assigning reg $t4
	  lw $t4, -8($fp)	# fill i to $t4 from $fp-8
# alloc_reg(); called for _tmp16, already in register $t3
	  add $t5, $t4, $t3	
# Free
# Free
# Free
	# i = _tmp17
# alloc_reg(); called for _tmp17, already in register $t5
# alloc_reg(); called for i, already in register $t4
	  move $t4, $t5 	# copy
# Free
# Free
	# Goto _L0
	  sw $t3, -76($fp)	# spill _tmp16 from $t3 to $fp-76
	  sw $t5, -80($fp)	# spill _tmp17 from $t5 to $fp-80
	  sw $t4, -8($fp)	# spill i from $t4 to $fp-8
	  b _L0		# unconditional branch
  _L1:
	# _tmp18 = 0
# alloc_reg(); called for _tmp18, assigning reg $t7
	  li $t7, 0		# load constant value 0 into $t7
# Free
	# Return _tmp18
# alloc_reg(); called for _tmp18, already in register $t7
	  move $v0, $t7
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  main:
	# BeginFunc 260
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 260	# decrement sp to make space for locals/temps
	# _tmp19 = 4
# alloc_reg(); called for _tmp19, assigning reg $t2
	  li $t2, 4		# load constant value 4 into $t2
# Free
	# _tmp20 = 0
# alloc_reg(); called for _tmp20, assigning reg $t0
	  li $t0, 0		# load constant value 0 into $t0
# Free
	# _tmp21 = _tmp19 < _tmp20
# alloc_reg(); called for _tmp21, assigning reg $t9
# alloc_reg(); called for _tmp19, already in register $t2
# alloc_reg(); called for _tmp20, already in register $t0
	  slt $t9, $t2, $t0	
# Free
# Free
# Free
	# IfZ _tmp21 Goto _L4
	  sw $t9, -20($fp)	# spill _tmp21 from $t9 to $fp-20
	  sw $t2, -12($fp)	# spill _tmp19 from $t2 to $fp-12
	  sw $t0, -16($fp)	# spill _tmp20 from $t0 to $fp-16
# alloc_reg(); called for _tmp21, already in register $t9
	  beqz $t9, _L4	# branch if _tmp21 is zero 
# Free
	# _tmp22 = "Decaf runtime error: Array size is <= 0\n"
	  .data			# create string constant marked with label
	  _string2: .asciiz "Decaf runtime error: Array size is <= 0\n"
	  .text
# alloc_reg(); called for _tmp22, assigning reg $t5
	  la $t5, _string2	# load label
# Free
	# PushParam _tmp22
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp22, already in register $t5
	  sw $t5, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t5, -24($fp)	# spill _tmp22 from $t5 to $fp-24
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# LCall _Halt
	  jal _Halt          	# jump to function
  _L4:
	# _tmp23 = 1
# alloc_reg(); called for _tmp23, assigning reg $t8
	  li $t8, 1		# load constant value 1 into $t8
# Free
	# _tmp24 = _tmp23 + _tmp19
# alloc_reg(); called for _tmp24, assigning reg $t7
# alloc_reg(); called for _tmp23, already in register $t8
# alloc_reg(); called for _tmp19, assigning reg $t2
	  lw $t2, -12($fp)	# fill _tmp19 to $t2 from $fp-12
	  add $t7, $t8, $t2	
# Free
# Free
# Free
	# _tmp25 = 4
# alloc_reg(); called for _tmp25, assigning reg $t9
	  li $t9, 4		# load constant value 4 into $t9
# Free
	# _tmp26 = _tmp24 * _tmp25
# alloc_reg(); called for _tmp26, assigning reg $t1
# alloc_reg(); called for _tmp24, already in register $t7
# alloc_reg(); called for _tmp25, already in register $t9
	  mul $t1, $t7, $t9	
# Free
# Free
# Free
	# PushParam _tmp26
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp26, already in register $t1
	  sw $t1, 4($sp)	# copy param value to stack
# Free
	# _tmp27 = LCall _Alloc
	  sw $t8, -28($fp)	# spill _tmp23 from $t8 to $fp-28
	  sw $t9, -36($fp)	# spill _tmp25 from $t9 to $fp-36
	  sw $t7, -32($fp)	# spill _tmp24 from $t7 to $fp-32
	  sw $t1, -40($fp)	# spill _tmp26 from $t1 to $fp-40
	  jal _Alloc         	# jump to function
# alloc_reg(); called for _tmp27, assigning reg $t2
	  move $t2, $v0		# copy function return value from $v0
# Free
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# *(_tmp27) = _tmp19
# alloc_reg(); called for _tmp19, assigning reg $t6
	  lw $t6, -12($fp)	# fill _tmp19 to $t6 from $fp-12
# alloc_reg(); called for _tmp27, already in register $t2
	  sw $t6, 0($t2) 	# store with offset
# Free
# Free
	# _tmp28 = _tmp27 + _tmp25
# alloc_reg(); called for _tmp28, assigning reg $t0
# alloc_reg(); called for _tmp27, already in register $t2
# alloc_reg(); called for _tmp25, assigning reg $t8
	  lw $t8, -36($fp)	# fill _tmp25 to $t8 from $fp-36
	  add $t0, $t2, $t8	
# Free
# Free
# Free
	# names = _tmp28
# alloc_reg(); called for _tmp28, already in register $t0
# alloc_reg(); called for names, assigning reg $t9
	  move $t9, $t0 	# copy
# Free
# Free
	# _tmp29 = 0
# alloc_reg(); called for _tmp29, assigning reg $t4
	  li $t4, 0		# load constant value 0 into $t4
# Free
	# _tmp30 = 0
# alloc_reg(); called for _tmp30, assigning reg $t3
	  li $t3, 0		# load constant value 0 into $t3
# Free
	# _tmp31 = _tmp29 < _tmp30
# alloc_reg(); called for _tmp31, assigning reg $t5
# alloc_reg(); called for _tmp29, already in register $t4
# alloc_reg(); called for _tmp30, already in register $t3
	  slt $t5, $t4, $t3	
# Free
# Free
# Free
	# _tmp32 = *(names + -4)
# alloc_reg(); called for _tmp32, assigning reg $t1
# alloc_reg(); called for names, already in register $t9
	  lw $t1, -4($t9) 	# load with offset
# Free
# Free
	# _tmp33 = _tmp29 < _tmp32
# alloc_reg(); called for _tmp33, assigning reg $t7
# alloc_reg(); called for _tmp29, already in register $t4
# alloc_reg(); called for _tmp32, already in register $t1
	  slt $t7, $t4, $t1	
# Free
# Free
# Free
	# _tmp34 = _tmp33 == _tmp30
# alloc_reg(); called for _tmp34, assigning reg $t6
# alloc_reg(); called for _tmp33, already in register $t7
# alloc_reg(); called for _tmp30, already in register $t3
	  seq $t6, $t7, $t3	
# Free
# Free
# Free
	# _tmp35 = _tmp31 || _tmp34
# alloc_reg(); called for _tmp35, assigning reg $t8
# alloc_reg(); called for _tmp31, already in register $t5
# alloc_reg(); called for _tmp34, already in register $t6
	  or $t8, $t5, $t6	
# Free
# Free
# Free
	# IfZ _tmp35 Goto _L5
	  sw $t2, -44($fp)	# spill _tmp27 from $t2 to $fp-44
	  sw $t0, -48($fp)	# spill _tmp28 from $t0 to $fp-48
	  sw $t9, -8($fp)	# spill names from $t9 to $fp-8
	  sw $t7, -68($fp)	# spill _tmp33 from $t7 to $fp-68
	  sw $t1, -64($fp)	# spill _tmp32 from $t1 to $fp-64
	  sw $t3, -56($fp)	# spill _tmp30 from $t3 to $fp-56
	  sw $t4, -52($fp)	# spill _tmp29 from $t4 to $fp-52
	  sw $t8, -76($fp)	# spill _tmp35 from $t8 to $fp-76
	  sw $t5, -60($fp)	# spill _tmp31 from $t5 to $fp-60
	  sw $t6, -72($fp)	# spill _tmp34 from $t6 to $fp-72
# alloc_reg(); called for _tmp35, already in register $t8
	  beqz $t8, _L5	# branch if _tmp35 is zero 
# Free
	# _tmp36 = "Decaf runtime error: Array subscript out of bound..."
	  .data			# create string constant marked with label
	  _string3: .asciiz "Decaf runtime error: Array subscript out of bounds\n"
	  .text
# alloc_reg(); called for _tmp36, assigning reg $t4
	  la $t4, _string3	# load label
# Free
	# PushParam _tmp36
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp36, already in register $t4
	  sw $t4, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t4, -80($fp)	# spill _tmp36 from $t4 to $fp-80
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# LCall _Halt
	  jal _Halt          	# jump to function
  _L5:
	# _tmp37 = 4
# alloc_reg(); called for _tmp37, assigning reg $t0
	  li $t0, 4		# load constant value 4 into $t0
# Free
	# _tmp38 = _tmp37 * _tmp29
# alloc_reg(); called for _tmp38, assigning reg $t2
# alloc_reg(); called for _tmp37, already in register $t0
# alloc_reg(); called for _tmp29, assigning reg $t5
	  lw $t5, -52($fp)	# fill _tmp29 to $t5 from $fp-52
	  mul $t2, $t0, $t5	
# Free
# Free
# Free
	# _tmp39 = names + _tmp38
# alloc_reg(); called for _tmp39, assigning reg $t8
# alloc_reg(); called for names, assigning reg $t9
	  lw $t9, -8($fp)	# fill names to $t9 from $fp-8
# alloc_reg(); called for _tmp38, already in register $t2
	  add $t8, $t9, $t2	
# Free
# Free
# Free
	# _tmp40 = "Brian"
	  .data			# create string constant marked with label
	  _string4: .asciiz "Brian"
	  .text
# alloc_reg(); called for _tmp40, assigning reg $t3
	  la $t3, _string4	# load label
# Free
	# *(_tmp39) = _tmp40
# alloc_reg(); called for _tmp40, already in register $t3
# alloc_reg(); called for _tmp39, already in register $t8
	  sw $t3, 0($t8) 	# store with offset
# Free
# Free
	# _tmp41 = 1
# alloc_reg(); called for _tmp41, assigning reg $t7
	  li $t7, 1		# load constant value 1 into $t7
# Free
	# _tmp42 = 0
# alloc_reg(); called for _tmp42, assigning reg $t4
	  li $t4, 0		# load constant value 0 into $t4
# Free
	# _tmp43 = _tmp41 < _tmp42
# alloc_reg(); called for _tmp43, assigning reg $t1
# alloc_reg(); called for _tmp41, already in register $t7
# alloc_reg(); called for _tmp42, already in register $t4
	  slt $t1, $t7, $t4	
# Free
# Free
# Free
	# _tmp44 = *(names + -4)
# alloc_reg(); called for _tmp44, assigning reg $t6
# alloc_reg(); called for names, already in register $t9
	  lw $t6, -4($t9) 	# load with offset
# Free
# Free
	# _tmp45 = _tmp41 < _tmp44
# alloc_reg(); called for _tmp45, assigning reg $t5
# alloc_reg(); called for _tmp41, already in register $t7
# alloc_reg(); called for _tmp44, already in register $t6
	  slt $t5, $t7, $t6	
# Free
# Free
# Free
	# _tmp46 = _tmp45 == _tmp42
# alloc_reg(); called for _tmp46, assigning reg $t9
# alloc_reg(); called for _tmp45, already in register $t5
# alloc_reg(); called for _tmp42, already in register $t4
	  seq $t9, $t5, $t4	
# Free
# Free
# Free
	# _tmp47 = _tmp43 || _tmp46
# alloc_reg(); called for _tmp47, assigning reg $t0, reg is dirty, spilling
	  sw $t0, -84($fp)	# spill _tmp37 from $t0 to $fp-84
# alloc_reg(); called for _tmp43, already in register $t1
# alloc_reg(); called for _tmp46, already in register $t9
	  or $t0, $t1, $t9	
# Free
# Free
# Free
	# IfZ _tmp47 Goto _L6
	  sw $t2, -88($fp)	# spill _tmp38 from $t2 to $fp-88
	  sw $t3, -96($fp)	# spill _tmp40 from $t3 to $fp-96
	  sw $t7, -100($fp)	# spill _tmp41 from $t7 to $fp-100
	  sw $t5, -116($fp)	# spill _tmp45 from $t5 to $fp-116
	  sw $t8, -92($fp)	# spill _tmp39 from $t8 to $fp-92
	  sw $t4, -104($fp)	# spill _tmp42 from $t4 to $fp-104
	  sw $t6, -112($fp)	# spill _tmp44 from $t6 to $fp-112
	  sw $t0, -124($fp)	# spill _tmp47 from $t0 to $fp-124
	  sw $t1, -108($fp)	# spill _tmp43 from $t1 to $fp-108
	  sw $t9, -120($fp)	# spill _tmp46 from $t9 to $fp-120
# alloc_reg(); called for _tmp47, already in register $t0
	  beqz $t0, _L6	# branch if _tmp47 is zero 
# Free
	# _tmp48 = "Decaf runtime error: Array subscript out of bound..."
	  .data			# create string constant marked with label
	  _string5: .asciiz "Decaf runtime error: Array subscript out of bounds\n"
	  .text
# alloc_reg(); called for _tmp48, assigning reg $t6
	  la $t6, _string5	# load label
# Free
	# PushParam _tmp48
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp48, already in register $t6
	  sw $t6, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t6, -128($fp)	# spill _tmp48 from $t6 to $fp-128
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# LCall _Halt
	  jal _Halt          	# jump to function
  _L6:
	# _tmp49 = 4
# alloc_reg(); called for _tmp49, assigning reg $t3
	  li $t3, 4		# load constant value 4 into $t3
# Free
	# _tmp50 = _tmp49 * _tmp41
# alloc_reg(); called for _tmp50, assigning reg $t2
# alloc_reg(); called for _tmp49, already in register $t3
# alloc_reg(); called for _tmp41, assigning reg $t1
	  lw $t1, -100($fp)	# fill _tmp41 to $t1 from $fp-100
	  mul $t2, $t3, $t1	
# Free
# Free
# Free
	# _tmp51 = names + _tmp50
# alloc_reg(); called for _tmp51, assigning reg $t0
# alloc_reg(); called for names, assigning reg $t7
	  lw $t7, -8($fp)	# fill names to $t7 from $fp-8
# alloc_reg(); called for _tmp50, already in register $t2
	  add $t0, $t7, $t2	
# Free
# Free
# Free
	# _tmp52 = "Cam"
	  .data			# create string constant marked with label
	  _string6: .asciiz "Cam"
	  .text
# alloc_reg(); called for _tmp52, assigning reg $t4
	  la $t4, _string6	# load label
# Free
	# *(_tmp51) = _tmp52
# alloc_reg(); called for _tmp52, already in register $t4
# alloc_reg(); called for _tmp51, already in register $t0
	  sw $t4, 0($t0) 	# store with offset
# Free
# Free
	# _tmp53 = 2
# alloc_reg(); called for _tmp53, assigning reg $t5
	  li $t5, 2		# load constant value 2 into $t5
# Free
	# _tmp54 = 0
# alloc_reg(); called for _tmp54, assigning reg $t6
	  li $t6, 0		# load constant value 0 into $t6
# Free
	# _tmp55 = _tmp53 < _tmp54
# alloc_reg(); called for _tmp55, assigning reg $t8
# alloc_reg(); called for _tmp53, already in register $t5
# alloc_reg(); called for _tmp54, already in register $t6
	  slt $t8, $t5, $t6	
# Free
# Free
# Free
	# _tmp56 = *(names + -4)
# alloc_reg(); called for _tmp56, assigning reg $t9
# alloc_reg(); called for names, already in register $t7
	  lw $t9, -4($t7) 	# load with offset
# Free
# Free
	# _tmp57 = _tmp53 < _tmp56
# alloc_reg(); called for _tmp57, assigning reg $t1
# alloc_reg(); called for _tmp53, already in register $t5
# alloc_reg(); called for _tmp56, already in register $t9
	  slt $t1, $t5, $t9	
# Free
# Free
# Free
	# _tmp58 = _tmp57 == _tmp54
# alloc_reg(); called for _tmp58, assigning reg $t7
# alloc_reg(); called for _tmp57, already in register $t1
# alloc_reg(); called for _tmp54, already in register $t6
	  seq $t7, $t1, $t6	
# Free
# Free
# Free
	# _tmp59 = _tmp55 || _tmp58
# alloc_reg(); called for _tmp59, assigning reg $t3, reg is dirty, spilling
	  sw $t3, -132($fp)	# spill _tmp49 from $t3 to $fp-132
# alloc_reg(); called for _tmp55, already in register $t8
# alloc_reg(); called for _tmp58, already in register $t7
	  or $t3, $t8, $t7	
# Free
# Free
# Free
	# IfZ _tmp59 Goto _L7
	  sw $t2, -136($fp)	# spill _tmp50 from $t2 to $fp-136
	  sw $t4, -144($fp)	# spill _tmp52 from $t4 to $fp-144
	  sw $t5, -148($fp)	# spill _tmp53 from $t5 to $fp-148
	  sw $t1, -164($fp)	# spill _tmp57 from $t1 to $fp-164
	  sw $t0, -140($fp)	# spill _tmp51 from $t0 to $fp-140
	  sw $t6, -152($fp)	# spill _tmp54 from $t6 to $fp-152
	  sw $t9, -160($fp)	# spill _tmp56 from $t9 to $fp-160
	  sw $t3, -172($fp)	# spill _tmp59 from $t3 to $fp-172
	  sw $t8, -156($fp)	# spill _tmp55 from $t8 to $fp-156
	  sw $t7, -168($fp)	# spill _tmp58 from $t7 to $fp-168
# alloc_reg(); called for _tmp59, already in register $t3
	  beqz $t3, _L7	# branch if _tmp59 is zero 
# Free
	# _tmp60 = "Decaf runtime error: Array subscript out of bound..."
	  .data			# create string constant marked with label
	  _string7: .asciiz "Decaf runtime error: Array subscript out of bounds\n"
	  .text
# alloc_reg(); called for _tmp60, assigning reg $t9
	  la $t9, _string7	# load label
# Free
	# PushParam _tmp60
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp60, already in register $t9
	  sw $t9, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t9, -176($fp)	# spill _tmp60 from $t9 to $fp-176
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# LCall _Halt
	  jal _Halt          	# jump to function
  _L7:
	# _tmp61 = 4
# alloc_reg(); called for _tmp61, assigning reg $t4
	  li $t4, 4		# load constant value 4 into $t4
# Free
	# _tmp62 = _tmp61 * _tmp53
# alloc_reg(); called for _tmp62, assigning reg $t2
# alloc_reg(); called for _tmp61, already in register $t4
# alloc_reg(); called for _tmp53, assigning reg $t8
	  lw $t8, -148($fp)	# fill _tmp53 to $t8 from $fp-148
	  mul $t2, $t4, $t8	
# Free
# Free
# Free
	# _tmp63 = names + _tmp62
# alloc_reg(); called for _tmp63, assigning reg $t3
# alloc_reg(); called for names, assigning reg $t5
	  lw $t5, -8($fp)	# fill names to $t5 from $fp-8
# alloc_reg(); called for _tmp62, already in register $t2
	  add $t3, $t5, $t2	
# Free
# Free
# Free
	# _tmp64 = "Gavan"
	  .data			# create string constant marked with label
	  _string8: .asciiz "Gavan"
	  .text
# alloc_reg(); called for _tmp64, assigning reg $t6
	  la $t6, _string8	# load label
# Free
	# *(_tmp63) = _tmp64
# alloc_reg(); called for _tmp64, already in register $t6
# alloc_reg(); called for _tmp63, already in register $t3
	  sw $t6, 0($t3) 	# store with offset
# Free
# Free
	# _tmp65 = 3
# alloc_reg(); called for _tmp65, assigning reg $t1
	  li $t1, 3		# load constant value 3 into $t1
# Free
	# _tmp66 = 0
# alloc_reg(); called for _tmp66, assigning reg $t9
	  li $t9, 0		# load constant value 0 into $t9
# Free
	# _tmp67 = _tmp65 < _tmp66
# alloc_reg(); called for _tmp67, assigning reg $t0
# alloc_reg(); called for _tmp65, already in register $t1
# alloc_reg(); called for _tmp66, already in register $t9
	  slt $t0, $t1, $t9	
# Free
# Free
# Free
	# _tmp68 = *(names + -4)
# alloc_reg(); called for _tmp68, assigning reg $t7
# alloc_reg(); called for names, already in register $t5
	  lw $t7, -4($t5) 	# load with offset
# Free
# Free
	# _tmp69 = _tmp65 < _tmp68
# alloc_reg(); called for _tmp69, assigning reg $t8
# alloc_reg(); called for _tmp65, already in register $t1
# alloc_reg(); called for _tmp68, already in register $t7
	  slt $t8, $t1, $t7	
# Free
# Free
# Free
	# _tmp70 = _tmp69 == _tmp66
# alloc_reg(); called for _tmp70, assigning reg $t5
# alloc_reg(); called for _tmp69, already in register $t8
# alloc_reg(); called for _tmp66, already in register $t9
	  seq $t5, $t8, $t9	
# Free
# Free
# Free
	# _tmp71 = _tmp67 || _tmp70
# alloc_reg(); called for _tmp71, assigning reg $t4, reg is dirty, spilling
	  sw $t4, -180($fp)	# spill _tmp61 from $t4 to $fp-180
# alloc_reg(); called for _tmp67, already in register $t0
# alloc_reg(); called for _tmp70, already in register $t5
	  or $t4, $t0, $t5	
# Free
# Free
# Free
	# IfZ _tmp71 Goto _L8
	  sw $t2, -184($fp)	# spill _tmp62 from $t2 to $fp-184
	  sw $t6, -192($fp)	# spill _tmp64 from $t6 to $fp-192
	  sw $t1, -196($fp)	# spill _tmp65 from $t1 to $fp-196
	  sw $t8, -212($fp)	# spill _tmp69 from $t8 to $fp-212
	  sw $t3, -188($fp)	# spill _tmp63 from $t3 to $fp-188
	  sw $t9, -200($fp)	# spill _tmp66 from $t9 to $fp-200
	  sw $t7, -208($fp)	# spill _tmp68 from $t7 to $fp-208
	  sw $t4, -220($fp)	# spill _tmp71 from $t4 to $fp-220
	  sw $t0, -204($fp)	# spill _tmp67 from $t0 to $fp-204
	  sw $t5, -216($fp)	# spill _tmp70 from $t5 to $fp-216
# alloc_reg(); called for _tmp71, already in register $t4
	  beqz $t4, _L8	# branch if _tmp71 is zero 
# Free
	# _tmp72 = "Decaf runtime error: Array subscript out of bound..."
	  .data			# create string constant marked with label
	  _string9: .asciiz "Decaf runtime error: Array subscript out of bounds\n"
	  .text
# alloc_reg(); called for _tmp72, assigning reg $t7
	  la $t7, _string9	# load label
# Free
	# PushParam _tmp72
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp72, already in register $t7
	  sw $t7, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t7, -224($fp)	# spill _tmp72 from $t7 to $fp-224
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# LCall _Halt
	  jal _Halt          	# jump to function
  _L8:
	# _tmp73 = 4
# alloc_reg(); called for _tmp73, assigning reg $t6
	  li $t6, 4		# load constant value 4 into $t6
# Free
	# _tmp74 = _tmp73 * _tmp65
# alloc_reg(); called for _tmp74, assigning reg $t2
# alloc_reg(); called for _tmp73, already in register $t6
# alloc_reg(); called for _tmp65, assigning reg $t0
	  lw $t0, -196($fp)	# fill _tmp65 to $t0 from $fp-196
	  mul $t2, $t6, $t0	
# Free
# Free
# Free
	# _tmp75 = names + _tmp74
# alloc_reg(); called for _tmp75, assigning reg $t4
# alloc_reg(); called for names, assigning reg $t1
	  lw $t1, -8($fp)	# fill names to $t1 from $fp-8
# alloc_reg(); called for _tmp74, already in register $t2
	  add $t4, $t1, $t2	
# Free
# Free
# Free
	# _tmp76 = "Julie"
	  .data			# create string constant marked with label
	  _string10: .asciiz "Julie"
	  .text
# alloc_reg(); called for _tmp76, assigning reg $t9
	  la $t9, _string10	# load label
# Free
	# *(_tmp75) = _tmp76
# alloc_reg(); called for _tmp76, already in register $t9
# alloc_reg(); called for _tmp75, already in register $t4
	  sw $t9, 0($t4) 	# store with offset
# Free
# Free
	  sw $t6, -228($fp)	# spill _tmp73 from $t6 to $fp-228
	  sw $t2, -232($fp)	# spill _tmp74 from $t2 to $fp-232
	  sw $t9, -240($fp)	# spill _tmp76 from $t9 to $fp-240
	  sw $t4, -236($fp)	# spill _tmp75 from $t4 to $fp-236
  _L9:
	# _tmp77 = 1
# alloc_reg(); called for _tmp77, assigning reg $t2
	  li $t2, 1		# load constant value 1 into $t2
# Free
	# IfZ _tmp77 Goto _L10
	  sw $t2, -244($fp)	# spill _tmp77 from $t2 to $fp-244
# alloc_reg(); called for _tmp77, already in register $t2
	  beqz $t2, _L10	# branch if _tmp77 is zero 
# Free
	# _tmp78 = "\nWho is your favorite CS143 staff member? "
	  .data			# create string constant marked with label
	  _string11: .asciiz "\nWho is your favorite CS143 staff member? "
	  .text
# alloc_reg(); called for _tmp78, assigning reg $t7
	  la $t7, _string11	# load label
# Free
	# PushParam _tmp78
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp78, already in register $t7
	  sw $t7, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t7, -248($fp)	# spill _tmp78 from $t7 to $fp-248
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp79 = LCall _ReadLine
	  jal _ReadLine      	# jump to function
# alloc_reg(); called for _tmp79, assigning reg $t4
	  move $t4, $v0		# copy function return value from $v0
# Free
	# PushParam _tmp79
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp79, already in register $t4
	  sw $t4, 4($sp)	# copy param value to stack
# Free
	# PushParam names
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for names, assigning reg $t2
	  lw $t2, -8($fp)	# fill names to $t2 from $fp-8
	  sw $t2, 4($sp)	# copy param value to stack
# Free
	# _tmp80 = LCall _Wild
	  sw $t4, -252($fp)	# spill _tmp79 from $t4 to $fp-252
	  jal _Wild          	# jump to function
# alloc_reg(); called for _tmp80, assigning reg $t4
	  move $t4, $v0		# copy function return value from $v0
# Free
	# PopParams 8
	  add $sp, $sp, 8	# pop params off stack
	# IfZ _tmp80 Goto _L11
	  sw $t4, -256($fp)	# spill _tmp80 from $t4 to $fp-256
# alloc_reg(); called for _tmp80, already in register $t4
	  beqz $t4, _L11	# branch if _tmp80 is zero 
# Free
	# _tmp81 = "You just earned 1000 bonus points!\n"
	  .data			# create string constant marked with label
	  _string12: .asciiz "You just earned 1000 bonus points!\n"
	  .text
# alloc_reg(); called for _tmp81, assigning reg $t7
	  la $t7, _string12	# load label
# Free
	# PushParam _tmp81
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp81, already in register $t7
	  sw $t7, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t7, -260($fp)	# spill _tmp81 from $t7 to $fp-260
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# Goto _L10
	  b _L10		# unconditional branch
  _L11:
	# _tmp82 = "That's not a good way to make points. Try again!\..."
	  .data			# create string constant marked with label
	  _string13: .asciiz "That's not a good way to make points. Try again!\n"
	  .text
# alloc_reg(); called for _tmp82, assigning reg $t2
	  la $t2, _string13	# load label
# Free
	# PushParam _tmp82
	  subu $sp, $sp, 4	# decrement sp to make space for param
# alloc_reg(); called for _tmp82, already in register $t2
	  sw $t2, 4($sp)	# copy param value to stack
# Free
	# LCall _PrintString
	  sw $t2, -264($fp)	# spill _tmp82 from $t2 to $fp-264
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# Goto _L9
	  b _L9		# unconditional branch
  _L10:
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
_PrintInt:
        subu $sp, $sp, 8
        sw $fp, 8($sp)
        sw $ra, 4($sp)
        addiu $fp, $sp, 8
        li   $v0, 1
        lw   $a0, 4($fp)
        syscall
        move $sp, $fp
        lw $ra, -4($fp)
        lw $fp, 0($fp)
        jr $ra
                                
_PrintString:
        subu $sp, $sp, 8
        sw $fp, 8($sp)
        sw $ra, 4($sp)
        addiu $fp, $sp, 8
        li   $v0, 4
        lw $a0, 4($fp)
        syscall
        move $sp, $fp
        lw $ra, -4($fp)
        lw $fp, 0($fp)
        jr $ra
        
_PrintBool:
	subu $sp, $sp, 8
	sw $fp, 8($sp)
	sw $ra, 4($sp)
        addiu $fp, $sp, 8
	lw $t1, 4($fp)
	blez $t1, fbr
	li   $v0, 4		# system call for print_str
	la   $a0, TRUE		# address of str to print
	syscall			
	b end
fbr:	li   $v0, 4		# system call for print_str
	la   $a0, FALSE		# address of str to print
	syscall				
end:	move $sp, $fp
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	jr $ra

_Alloc:
        subu $sp, $sp, 8
        sw $fp, 8($sp)
        sw $ra, 4($sp)
        addiu $fp, $sp, 8
        li   $v0, 9
        lw $a0, 4($fp)
	syscall
        move $sp, $fp
        lw $ra, -4($fp)
        lw $fp, 0($fp) 
        jr $ra


_StringEqual:
	subu $sp, $sp, 8      # decrement sp to make space to save ra, fp
	sw $fp, 8($sp)        # save fp
	sw $ra, 4($sp)        # save ra
	addiu $fp, $sp, 8     # set up new fp
	subu $sp, $sp, 4      # decrement sp to make space for locals/temps

	li $v0,0

	#Determine length string 1
	lw $t0, 4($fp)       
	li $t3,0
bloop1: 
	lb $t5, ($t0) 
	beqz $t5, eloop1	
	addi $t0, 1
	addi $t3, 1
	b bloop1
eloop1:

	#Determine length string 2
	lw $t1, 8($fp)
	li $t4,0
bloop2: 
	lb $t5, ($t1) 
	beqz $t5, eloop2	
	addi $t1, 1
	addi $t4, 1
	b bloop2
eloop2:
	bne $t3,$t4,end1       #Check String Lengths Same

	lw $t0, 4($fp)       
	lw $t1, 8($fp)
	li $t3, 0     		
bloop3:	
	lb $t5, ($t0) 
	lb $t6, ($t1) 
	bne $t5, $t6, end1
	addi $t3, 1
	addi $t0, 1
	addi $t1, 1
	bne $t3,$t4,bloop3
eloop3:	li $v0,1

end1:	move $sp, $fp         # pop callee frame off stack
	lw $ra, -4($fp)       # restore saved ra
	lw $fp, 0($fp)        # restore saved fp
	jr $ra                # return from function

_Halt:
        li $v0, 10
        syscall

_ReadInteger:
	subu $sp, $sp, 8      # decrement sp to make space to save ra, fp
	sw $fp, 8($sp)        # save fp
	sw $ra, 4($sp)        # save ra
	addiu $fp, $sp, 8     # set up new fp
	subu $sp, $sp, 4      # decrement sp to make space for locals/temps
	li $v0, 5
	syscall
	move $sp, $fp         # pop callee frame off stack
	lw $ra, -4($fp)       # restore saved ra
	lw $fp, 0($fp)        # restore saved fp
	jr $ra
        

_ReadLine:
	subu $sp, $sp, 8      # decrement sp to make space to save ra, fp
	sw $fp, 8($sp)        # save fp
	sw $ra, 4($sp)        # save ra
	addiu $fp, $sp, 8     # set up new fp
	subu $sp, $sp, 4      # decrement sp to make space for locals/temps
	li $a1, 40
	la $a0, SPACE
	li $v0, 8
	syscall

	la $t1, SPACE
bloop4: 
	lb $t5, ($t1) 
	beqz $t5, eloop4	
	addi $t1, 1
	b bloop4
eloop4:
	addi $t1,-1
	li $t6,0
        sb $t6, ($t1)

	la $v0, SPACE
	move $sp, $fp         # pop callee frame off stack
	lw $ra, -4($fp)       # restore saved ra
	lw $fp, 0($fp)        # restore saved fp
	jr $ra
	

	.data
TRUE:.asciiz "true"
FALSE:.asciiz "false"
SPACE:.asciiz "Making Space For Inputed Values Is Fun."
