! Name of package being compiled: aFunProgram
! 
! Symbols from runtime.s
	.import	_putString
	.import	_heapInitialize
	.import	_heapAlloc
	.import	_heapFree
	.import	_IsKindOf
	.import	_RestoreCatchStack
	.import	_PerformThrow
	.import	_runtimeErrorOverflow
	.import	_runtimeErrorZeroDivide
	.import	_runtimeErrorNullPointer
	.import	_runtimeErrorUninitializedObject
	.import	_runtimeErrorWrongObject
	.import	_runtimeErrorWrongObject2
	.import	_runtimeErrorWrongObject3
	.import	_runtimeErrorBadObjectSize
	.import	_runtimeErrorDifferentArraySizes
	.import	_runtimeErrorWrongArraySize
	.import	_runtimeErrorUninitializedArray
	.import	_runtimeErrorBadArrayIndex
	.import	_runtimeErrorNullPointerDuringCall
	.import	_runtimeErrorArrayCountNotPositive
	.import	_runtimeErrorRestoreCatchStackError
	.text
! ErrorDecls
	.import	_Error_P_System_UncaughtThrowError
	.align
! Functions imported from other packages
	.import	GetCh
	.import	print
	.import	printInt
	.import	printHex
	.import	printChar
	.import	printBool
	.import	printDouble
	.import	_P_System_nl
	.import	RuntimeExit
	.import	getCatchStack
	.import	MemoryZero
	.import	MemoryCopy
	.import	_P_System_KPLMemoryInitialize
	.import	_P_System_KPLMemoryAlloc
	.import	_P_System_KPLMemoryFree
	.import	_P_System_KPLUncaughtThrow
	.import	_P_System_KPLIsKindOf
	.import	_P_System_KPLSystemError
! Externally visible functions in this package
	.export	_mainEntry
	.export	main
! The following class and its methods are from other packages
	.import	_P_System_Object
! The following interfaces are from other packages
! The following interfaces are from this package
! Globals imported from other packages
! Global variables in this package
	.data
	.align
	.text
! 
! =====  MAIN ENTRY POINT  =====
! 
_mainEntry:
	set	_packageName,r2		! Get CheckVersion started
	set	0xb11804f8,r3		! .  hashVal = -1323825928
	call	_CheckVersion_P_aFunProgram_	! .
	cmp	r1,0			! .
	be	_Label_1		! .
	ret				! .
_Label_1:				! .
	call	_heapInitialize
	jmp	main
! 
! Source Filename and Package Name
! 
_sourceFileName:
	.ascii	"aFunProgram.k\0"
_packageName:
	.ascii	"aFunProgram\0"
	.align
!
! CheckVersion
!
!     This routine is passed:
!       r2 = ptr to the name of the 'using' package
!       r3 = the expected hashVal for 'used' package (myPackage)
!     It prints an error message if the expected hashVal is not correct
!     It then checks all the packages that 'myPackage' uses.
!
!     This routine returns:
!       r1:  0=No problems, 1=Problems
!
!     Registers modified: r1-r4
!
_CheckVersion_P_aFunProgram_:
	.export	_CheckVersion_P_aFunProgram_
	set	0xb11804f8,r4		! myHashVal = -1323825928
	cmp	r3,r4
	be	_Label_2
	set	_CVMess1,r1
	call	_putString
	mov	r2,r1			! print using package
	call	_putString
	set	_CVMess2,r1
	call	_putString
	set	_packageName,r1		! print myPackage
	call	_putString
	set	_CVMess3,r1
	call	_putString
	set	_packageName,r1		! print myPackage
	call	_putString
	set	_CVMess4,r1
	call	_putString
	mov	r2,r1			! print using package
	call	_putString
	set	_CVMess5,r1
	call	_putString
	set	_packageName,r1		! print myPackage
	call	_putString
	set	_CVMess6,r1
	call	_putString
	mov	1,r1
	ret	
_Label_2:
	mov	0,r1
! Make sure _P_System_ has hash value 0x5af73160 (decimal 1526149472)
	set	_packageName,r2
	set	0x5af73160,r3
	call	_CheckVersion_P_System_
	.import	_CheckVersion_P_System_
	cmp	r1,0
	bne	_Label_3
_Label_3:
	ret
_CVMess1:	.ascii	"\nVERSION CONSISTENCY ERROR: Package '\0"
_CVMess2:	.ascii	"' uses package '\0"
_CVMess3:	.ascii	"'.  Whenever a header file is modified, all packages that use that package (directly or indirectly) must be recompiled.  The header file for '\0"
_CVMess4:	.ascii	"' has been changed since '\0"
_CVMess5:	.ascii	"' was compiled last.  Please recompile all packages that depend on '\0"
_CVMess6:	.ascii	"'.\n\n\0"
	.align
! 
! ===============  FUNCTION main  ===============
! 
main:
	push	r14
	mov	r15,r14
	push	r13
	set	_RoutineDescriptor_main,r1
	push	r1
	mov	3,r1
_Label_22:
	push	r0
	sub	r1,1,r1
	bne	_Label_22
	mov	5,r13		! source line 5
	mov	"\0\0FU",r10
! VARIABLE INITIALIZATION...
! first
!   first = 1		(1 byte)
	mov	1,r1
	storeb	r1,[r14+-12]
! WHILE STATEMENT...
	mov	9,r13		! source line 9
	mov	"\0\0WH",r10
_Label_4:
!	_Label_5	jmp	_Label_5
_Label_5:
	mov	9,r13		! source line 9
	mov	"\0\0WB",r10
! ASSIGNMENT STATEMENT...
	mov	10,r13		! source line 10
	mov	"\0\0AS",r10
!   Call the function
	mov	10,r13		! source line 10
	mov	"\0\0CE",r10
	call	GetCh
!   Retrieve Result: targetName=_temp_7  sizeInBytes=4
	load	[r15],r1
	store	r1,[r14+-16]
!   ch = intToChar (_temp_7)
	load	[r14+-16],r1
	storeb	r1,[r14+-11]
! IF STATEMENT...
	mov	11,r13		! source line 11
	mov	"\0\0IF",r10
!   if first then goto _Label_10 else goto _Label_9
	loadb	[r14+-12],r1
	cmp	r1,0
	be	_Label_9
	jmp	_Label_10
_Label_10:
!   _temp_11 = ch XOR 113		(bool)
	loadb	[r14+-11],r1
	mov	113,r2
	xor	r1,r2,r1
	storeb	r1,[r14+-10]
!   if _temp_11 then goto _Label_9 else goto _Label_8
	loadb	[r14+-10],r1
	cmp	r1,0
	be	_Label_8
	jmp	_Label_9
_Label_8:
! THEN...
	mov	12,r13		! source line 12
	mov	"\0\0TN",r10
! BREAK STATEMENT...
	mov	12,r13		! source line 12
	mov	"\0\0BR",r10
	jmp	_Label_6
! END IF...
_Label_9:
! IF STATEMENT...
	mov	18,r13		! source line 18
	mov	"\0\0IF",r10
!   _temp_14 = ch XOR 10		(bool)
	loadb	[r14+-11],r1
	mov	10,r2
	xor	r1,r2,r1
	storeb	r1,[r14+-9]
!   if _temp_14 then goto _Label_13 else goto _Label_12
	loadb	[r14+-9],r1
	cmp	r1,0
	be	_Label_12
	jmp	_Label_13
_Label_12:
! THEN...
	mov	19,r13		! source line 19
	mov	"\0\0TN",r10
! ASSIGNMENT STATEMENT...
	mov	19,r13		! source line 19
	mov	"\0\0AS",r10
!   first = 1		(1 byte)
	mov	1,r1
	storeb	r1,[r14+-12]
	jmp	_Label_15
_Label_13:
! ELSE...
	mov	21,r13		! source line 21
	mov	"\0\0EL",r10
! ASSIGNMENT STATEMENT...
	mov	21,r13		! source line 21
	mov	"\0\0AS",r10
!   first = 0		(1 byte)
	mov	0,r1
	storeb	r1,[r14+-12]
! END IF...
_Label_15:
! CALL STATEMENT...
!   Prepare Argument: offset=8  value=ch  sizeInBytes=1
	loadb	[r14+-11],r1
	storeb	r1,[r15+0]
!   Call the function
	mov	23,r13		! source line 23
	mov	"\0\0CE",r10
	call	printChar
! END WHILE...
	jmp	_Label_4
_Label_6:
! --------------------  DEBUG  --------------------
	mov	26,r13		! source line 26
	mov	"\0\0DE",r10
	debug
! RETURN STATEMENT...
	mov	26,r13		! source line 26
	mov	"\0\0RE",r10
	add	r15,16,r15
	pop	r13
	pop	r14
	ret
! 
! Routine Descriptor
! 
_RoutineDescriptor_main:
	.word	_sourceFileName
	.word	_Label_16
	.word	0		! total size of parameters
	.word	12		! frame size = 12
	.word	_Label_17
	.word	-9
	.word	1
	.word	_Label_18
	.word	-10
	.word	1
	.word	_Label_19
	.word	-16
	.word	4
	.word	_Label_20
	.word	-11
	.word	1
	.word	_Label_21
	.word	-12
	.word	1
	.word	0
_Label_16:
	.ascii	"main\0"
	.align
_Label_17:
	.byte	'C'
	.ascii	"_temp_14\0"
	.align
_Label_18:
	.byte	'C'
	.ascii	"_temp_11\0"
	.align
_Label_19:
	.byte	'?'
	.ascii	"_temp_7\0"
	.align
_Label_20:
	.byte	'C'
	.ascii	"ch\0"
	.align
_Label_21:
	.byte	'B'
	.ascii	"first\0"
	.align
