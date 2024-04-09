!KPL Programming Language Runtime Support Code
!
! ==========  THIS VERSION IS TO SUPPORT PROJECT 1  ==========
!
! Harry Porter  -  09/26/04
!
! The following functions are implemented in this file and may be used by
! the KPL programmer.  They follow the instructions provided by WWU
! and may contain errors.
!
	.export GetCh
!
! The following functions are implemented in this file and may be used by
! the KPL programmer.  They follow the standard KPL calling
! conventions.
!
	.export	print
	.export	printInt
	.export	printHex
	.export	printChar
	.export	printBool
	.export	printDouble
	.export	MemoryZero
	.export	MemoryCopy
	.export	getCatchStack
	.export	RuntimeExit
!
! The following functions are implemented in this file and
! are used by the code generated by the KPL compiler:
!
	.export _putString
	.export	_heapInitialize
	.export	_heapAlloc
	.export	_heapFree
	.export	_RestoreCatchStack
	.export	_PerformThrow
	.export	_IsKindOf
	.export	_PosInf
	.export	_NegInf
	.export	_NegZero
!
! Jumps to the following labels may be generated by the KPL compiler.
! Each will print an error message and halt program execution.
!
	.export	_runtimeErrorOverflow
	.export	_runtimeErrorZeroDivide
	.export	_runtimeErrorNullPointer
	.export	_runtimeErrorUninitializedObject
	.export	_runtimeErrorWrongObject
	.export	_runtimeErrorWrongObject2
	.export	_runtimeErrorWrongObject3
	.export	_runtimeErrorBadObjectSize
	.export	_runtimeErrorDifferentArraySizes
	.export	_runtimeErrorWrongArraySize
	.export	_runtimeErrorUninitializedArray
	.export	_runtimeErrorBadArrayIndex
	.export	_runtimeErrorNullPointerDuringCall
	.export	_runtimeErrorArrayCountNotPositive
	.export	_runtimeErrorRestoreCatchStackError
!
! Jumps to the following labels may be generated occur only in this file.
!
!	_runtimeErrorThrowHandlerHasReturned
!	_runtimeErrorFatalThrowError
!	_runtimeErrorInterruptsEnabled
!
! Routines that are created by the KPL Compiler and called from here:
!
	.import _mainEntry
!
! Symbols that are created by the KPL compiler and used in this file:
!
	.import _Error_P_System_UncaughtThrowError
!
! Routines that should be implemented in KPL and called from here:
!
	.import _P_System_KPLMemoryInitialize
	.import _P_System_KPLMemoryAlloc
	.import _P_System_KPLMemoryFree
	.import _P_System_KPLUncaughtThrow
	.import _P_System_KPLIsKindOf
!
! The initial stack will start at high memory and grow downward:
!
STACK_START	=	0x00ffff00



!
! =====================  Program entry point  =====================
!
	.text
_entry:



!
! =====================  Interrupt Trap Vector (in low memory)  =====================
!
! Here is the interrupt vector, which will be loaded at address 0x00000000.
! Each entry is 4 bytes.  They are located at fixed, pre-defined addresses.
! Each entry contains a jump to the handler routine.
!
! Timer interrupts will occur, but this code will ignore them be returning.
! None of the other interrupts should occur; if they do, we will print an error
! message and halt.
!
PowerOnReset:
        jmp     RuntimeStartup
TimerInterrupt:
        reti
DiskInterrupt:
        jmp     DiskInterruptHandler
SerialInterrupt:
        jmp     SerialInterruptHandler
HardwareFault:
        jmp     HardwareFaultHandler
IllegalInstruction:
        jmp     IllegalInstructionHandler
ArithmeticException:
        jmp     ArithmeticExceptionHandler
AddressException:
        jmp     AddressExceptionHandler
PageInvalidException:
        jmp     PageInvalidExceptionHandler
PageReadonlyException:
        jmp     PageReadonlyExceptionHandler
PrivilegedInstruction:
        jmp     PrivilegedInstructionHandler
AlignmentException:
        jmp     AlignmentExceptionHandler
ExceptionDuringInterrupt:
        jmp     ExceptionDuringInterruptHandler
SyscallTrap:
        jmp     SyscallTrapHandler




! 
! =====================  Interrupt Handlers  =====================
! 
DiskInterruptHandler:
	set	ExceptMess1,r10			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

SerialInterruptHandler:
	set	ExceptMess1,r11			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

SyscallTrapHandler:
	set	ExceptMess1,r12			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

HardwareFaultHandler:
	set	ExceptMess1,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

IllegalInstructionHandler:
	set	ExceptMess2,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

ArithmeticExceptionHandler:
	set	ExceptMess3,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

AddressExceptionHandler:
	set	ExceptMess4,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

PageInvalidExceptionHandler:
	set	ExceptMess5,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

PageReadonlyExceptionHandler:
	set	ExceptMess6,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

PrivilegedInstructionHandler:
	set	ExceptMess7,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

AlignmentExceptionHandler:
	set	ExceptMess8,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

ExceptionDuringInterruptHandler:
	set	STACK_START,r15			! Reset stack so printing works
	set	ExceptMess9,r1			! Print an error message
	jmp	printRuntimeError		! .  and terminate all execution

ExceptMess1:
	.ascii	"\nA HardwareFaultException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess2:
	.ascii	"\nAn IllegalInstructionException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess3:
	.ascii	"\nAn ArithmeticException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess4:
	.ascii	"\nAn AddressException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess5:
	.ascii	"\nA PageInvalidException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess6:
	.ascii	"\nA PageReadonlyException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess7:
	.ascii	"\nA PrivilegedInstruction has occurred!  Type 'st' to see stack.\n\0"
ExceptMess8:
	.ascii	"\nAn AlignmentException has occurred!  Type 'st' to see stack.\n\0"
ExceptMess9:
	.ascii	"\nA ExceptionDuringInterrupt has occurred!  Type 'st' to see stack.\n\0"
ExceptMess10:
	.ascii	"\nA DiskInterrupt has occurred!  Type 'st' to see stack.\n\0"
ExceptMess11:
	.ascii	"\nA SerialInterrupt has occurred!  Type 'st' to see stack.\n\0"
ExceptMess12:
	.ascii	"\nA SyscallTrap has occurred!  Type 'st' to see stack.\n\0"
	.align



! 
! =====================  KPL Runtime Error Handlers  =====================
! 
_runtimeErrorOverflow:
	set	ErrorMess1,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorZeroDivide:
	set	ErrorMess2,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorNullPointer:
	set	ErrorMess3,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorUninitializedObject:
	set	ErrorMess4,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorWrongObject:
	set	ErrorMess5,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorWrongObject2:
	set	ErrorMess6,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorWrongObject3:
	set	ErrorMess7,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorBadObjectSize:
	set	ErrorMess8,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorDifferentArraySizes:
	call	stopInterrupts		! suspend any thread switching
	push	r1			! save r1 and r3
	push	r3			! .  so the user can look at them
	set	ErrorMess9,r1		! print the error message
	call	_putString		! .
	pop	r3			! restore r1 and r3
	pop	r1			! .
	jmp	TerminateRuntime	! Can't continue: invoke "debug"

_runtimeErrorWrongArraySize:
	call	stopInterrupts		! suspend any thread switching
	push	r1			! save r1 and r2
	push	r2			! .  so the user can look at them
	set	ErrorMess10,r1		! print the error message
	call	_putString		! .
	pop	r2			! restore r1 and r2
	pop	r1			! .
	jmp	TerminateRuntime	! terminate all execution

_runtimeErrorUninitializedArray:
	set	ErrorMess11a,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorBadArrayIndex:
	call	stopInterrupts		! suspend any thread switching
	push	r2			! save r2 so the user can look at it
	set	ErrorMess11,r1		! print the error message
	call	_putString		! .
	pop	r2			! restore r2
	jmp	TerminateRuntime	! terminate all execution

_runtimeErrorNullPointerDuringCall:
	set	ErrorMess12,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorArrayCountNotPositive:
	call	stopInterrupts		! suspend any thread switching
	push	r1			! save r1 so the user can look at it
	set	ErrorMess13,r1		! print the error message
	call	_putString		! .
	pop	r1			! restore r1
	jmp	TerminateRuntime	! terminate all execution

_runtimeErrorRestoreCatchStackError:
	set	ErrorMess14,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorThrowHandlerHasReturned:
	set	ErrorMess15,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorFatalThrowError:
	set	ErrorMess16,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution

_runtimeErrorInterruptsEnabled:
	set	ErrorMess17,r1		! Print an error message
	jmp	printRuntimeError	! .  and terminate all execution


ErrorMess1:
	.ascii	"\nArithmetic overflow has occurred!  Type 'st' to see stack.\n\0"
ErrorMess2:
	.ascii	"\nDivide-by-zero has occurred!  Type 'st' to see stack.\n\0"
ErrorMess3:
	.ascii	"\nAttempt to use a null pointer!  Type 'st' to see stack.\n\0"
ErrorMess4:
	.ascii	"\nAttempt to use an uninitialized object!  Type 'st' to see stack.\n\0"
ErrorMess5:
	.ascii	"\nDuring an assignment of the form '*ptr = x', the ptr does not already point to an instance of the same class as x!  Type 'st' to see stack.\n\0"
ErrorMess6:
	.ascii	"\nDuring an assignment of the form 'x = *ptr', the ptr does not already point to an instance of the same class as x!  Type 'st' to see stack.\n\0"
ErrorMess7:
	.ascii	"\nDuring an object assignment of the form '*ptr1 = *ptr2', the two objects are not instances of the same class!  Type 'st' to see stack.\n\0"
ErrorMess8:
	.ascii	"\nDuring an object assignment or object equality test, something is wrong with the dispatch table pointer, the dispatch table, "
	.ascii   "or the class descriptor; the size of the object is less than 4!  Type 'st' to see stack.\n\0"
ErrorMess9:
	.ascii	"\nDuring an array copy, the two arrays have different sizes  (r1=target size, r3=source size)!  Type 'st' to see stack.\n\0"
ErrorMess10:
	.ascii	"\nDuring an array copy, a dynamic array does not have the correct size (r1=actual size, r2=expected size)!  Type 'st' to see stack.\n\0"
ErrorMess11:
	.ascii	"\nDuring an array index calculation, the index is either less than 0 or greater than or equal to the array size (r2=index)!  Type 'st' to see stack.\n\0"
ErrorMess11a:
	.ascii	"\nAttempt to use an uninitialized array!  Type 'st' to see stack.\n\0"
ErrorMess12:
	.ascii	"\nDuring the invocation of a nameless function, the function pointer was null!  Type 'st' to see stack.\n\0"
ErrorMess13:
	.ascii	"\nDuring the initialization of an array, a 'count' expression was zero or less (r1=count)!  Type 'st' to see stack.\n\0"
	.align
ErrorMess14:
	.ascii	"\nWhile popping the Catch Stack, an error has occurred!  Type 'st' to see stack.\n\0"
ErrorMess15:
	.ascii	"\nAttempt to return from KPLUncaughtThrow!  Type 'st' to see stack.\n\0"
ErrorMess16:
	.ascii	"\nError 'UncaughtThrowError' has been thrown but not caught!  Type 'st' to see stack.\n\0"
ErrorMess17:
	.ascii	"\nAttempt to perform serial I/O while interrupts are not disabled!  Type 'st' to see stack.\n\0"
	.align



! 
! =====================  printRuntimeError  =====================
! 
!
! Come here to print a runtime error message and die.
! On entry, r1 points to the message.
!
printRuntimeError:
	call	stopInterrupts		! suspend any thread switching
	call	_putString		! .
	jmp	TerminateRuntime	! Can't continue: invoke "debug"



! 
! =====================  stopInterrupts  =====================
! 
! This routine is called when a runtime error occurs; we are planning to
! print a final error message and suspend execution.  The problem is that
! the running program may be an operating system with multiple threads.
! An interrupt could occur during the printing of the error message, causing
! a thread switch, which might result in incomplete or incorrect execution of
! the code to print the message.
!
! This routine disables interrutps, so that the message can be printed safely.
! When this routine is called, interrupts may or may not be enabled.  Also we
! may or may not be in System mode, we cannot simply execute a "cleari" instruction.
!
! This routine overwrites the syscall interrupt vector and then executes a
! syscall instruction.  This will have the effect of clearing the "I" bit and
! setting the "S" bit, achieving the desired effect.
!
! Registers modified: none.
!
stopInterrupts:
	bis	needToClear		! If interrupts are disabled  &&
	bsc	needToClear		! .  we are in System mode
	ret				!   return
needToClear:				! EndIf
	push	r1			! Save r1 and r2
	push	r2			! .
	set	relAddr,r1		! Move a rel addr of keepGoing into r1
	load	[r1],r1			! .
	set	0xa1000000,r2		! Add in "jmp" op code
	or	r1,r2,r1		! .
	store	r1,[r0+0x00000034]	! Store that jmp in the syscall vector
	syscall	0			! Execute a syscall trap

relAddr:
	.word	(keepGoing-0x00000034)

keepGoing:
	pop	r0			! The interrupt pushed 3 words onto
	pop	r0			! .  the stack; get rid of them.
	pop	r0			! .
	pop	r2			! Restore r1 and r2
	pop	r1			! .
	ret				! return to caller



!
! Misc Floating-Point Constants
!
_PosInf:
	.word	0x7ff00000
	.word	0x00000000
_NegInf:
	.word	0xfff00000
	.word	0x00000000
_NegZero:
	.word	0x80000000
	.word	0x00000000



!
! Name of this file
!
SourceFileName:
	.ascii	"Runtime.s\0"
	.align



! 
! =====================  RuntimeStartup  =====================
!
! Prepare for KPL program execution.  Initialize the stack pointer,
! print the "PROGRAM STARTING" message, and call the "main" function.
!
RuntimeStartup:
	set	STACK_START,r15		! Initialize the stack pointer
	set	startupMessage,r1	! Print start-up message
	call	_putString		! .
					! Interrupts are initially disabled
	call	_mainEntry		! Call "_mainEntry"
	jmp	RuntimeExit		! Perform normal termination
startupMessage:
	.ascii	"====================  KPL PROGRAM STARTING  ====================\n\0"
	.align



! 
! =====================  RuntimeExit  =====================
! 
! external RuntimeExit ()
!
! This routine prints the normal program termination message and
! never returns.  This routine is also callable from KPL code.
!
RuntimeExit:
	call	stopInterrupts		! suspend any thread switching
	set	goodbyeMessage,r1	! print message
	call	_putString		! .
	jmp	TerminateRuntime	! goto TerminateRuntime
goodbyeMessage:
	.ascii	"\n====================  KPL PROGRAM TERMINATION  ====================\n\0"
	.align



!
! =====================  TerminateRuntime  =====================
!
! Come here when there is nothing more that can be done.
! NOTE: Interrupts must be disabled before calling this routine, to
! prevent any further thread switching.
!
TerminateRuntime:
	debug				! Pop up to BLITZ Emulator command mode
	set	noGoMessage,r1		! Print "You may not continue"
	call	_putString		! .
	jmp	TerminateRuntime	! ... and repeat, if "go" is entered
noGoMessage:
	.ascii	"\nThe KPL program has terminated; you may not continue!\n\0"
	.align



! 
! =====================  _putString  =====================
! 
! This routine is passed r1 = a pointer to a string of characters, terminated
! by '\0'.  It prints all of them except the final '\0'.  The string is printed
! atomically by calling 'debug2'.
!
! r1: ptr to string
! r2: ptr to string (saved version)
! r3: count
! r4: character 
!
! Registers modified: none
!
_putString:
	push	r1			! save registers
	push	r2			! .
	push	r3			! .
	push	r4			! .
	mov	r1,r2			! r2 := ptr to the string
	mov	0,r3			! r3 := count of characters
putStLoop:				! loop
	loadb	[r1],r4			!   r4 := next char
	cmp	r4,0			!   if (r4 == '\0')
	be	putStExit		!     then break
	add	r1,1,r1			!   incr ptr
	add	r3,1,r3			!   incr count
	jmp	putStLoop		! end
putStExit:				! .
	mov	2,r1			! perform upcall to emulator to
	debug2				! . do the printing
	pop	r4			! restore regs
	pop	r3			! .
	pop	r2			! .
	pop	r1			! .
	ret				! return



! 
! =====================  getCatchStack  =====================
! 
! external getCatchStack () returns ptr to CATCH_RECORD
!
! This routine returns the value of r12, which points to the Catch-Stack.
! The Catch-Stack is a linked list of CATCH_RECORDs, possibly NULL.
!
! NOTE:  Whenever we leave the body statements in a try (i.e., fall-thru,
!        throw, or return), records from the catch stack will be popped and
!        freed.  "getCatchStack" returns a pointer to a list of CATCH_RECORDs
!        as it is when "getCatchStack" is called, so watch out that the caller
!        does not use records after they are freed.
! 
getCatchStack:
		store	r12,[r15+4]		! put r12 in the result position
		ret				! return




! 
! =====================  MemoryZero  =====================
! 
! external MemoryZero (p: ptr to void, byteCount: int)
!
! This function is passed "p" = a pointer, and "byteCount" = the number
! of bytes.  It sets that block of memory to all zeros.  The byteCount may
! be zero or negative, in which case this routine zeroes nothing.  This
! routine performs no other error checking.
!
! Here is the algorithm:
!
!      while byteCount > 0 && p asInteger % 4 != 0
!        *(p asPtrTo char) = '\x00'
!        p = p + 1
!        byteCount = byteCount - 1
!      endWhile
!      while byteCount > 3
!        *(p asPtrTo int) = 0
!        p = p + 4
!        byteCount = byteCount - 4
!      endWhile
!      if byteCount == 3
!        *(p asPtrTo char) = '\x00'
!        *((p+1) asPtrTo char) = '\x00'
!        *((p+2) asPtrTo char) = '\x00'
!      elseIf byteCount == 2
!        *(p asPtrTo char) = '\x00'
!        *((p+1) asPtrTo char) = '\x00'
!      elseIf byteCount == 1
!        *(p asPtrTo char) = '\x00'
!      endIf
!
! Register usage:
!   r1 = p
!   r2 = byteCount
!   r3 = work reg
!
MemoryZero:
		load	[r15+4],r1		! r1 = arg1 (p)
		load	[r15+8],r2		! r2 = arg2 (byteCount)
mzLoop1:					! LOOP:
		cmp	r2,0			!   if byteCount <= 0 exit
		ble	mzLoop2Test		!   .
		and	r1,0x00000003,r3	!   tmp = p % 4
		cmp	r3,0			!   if tmp == 0 exit
		be	mzLoop2Test		!   .
		storeb	r0,[r1]			!   *p = 0x00
		add	r1,1,r1			!   p = p + 1
		sub	r2,1,r2			!   byteCount = byteCount - 1
		jmp	mzLoop1			! ENDLOOP
!		jmp	mzLoop2Test		! LOOP
mzLoop2:					! .
		store	r0,[r1]			!   *p = 0x00000000
		add	r1,4,r1			!   p = p + 4
		sub	r2,4,r2			!   byteCount = byteCount - 4
mzLoop2Test:
		cmp	r2,3			!   if byteCount > 3 then repeat
		bg	mzLoop2			!   .

		cmp	r2,3			! if byteCount == 3
		bne	mzTry2			! .
		storeb	r0,[r1]			!   *p = 0x00
		storeb	r0,[r1+1]		!   *(p+1) = 0x00
		storeb	r0,[r1+2]		!   *(p+2) = 0x00
		ret				!   return
mzTry2:		cmp	r2,2			! else if byteCount == 2
		bne	mzTry1			! .
		storeb	r0,[r1]			!   *p = 0x00
		storeb	r0,[r1+1]		!   *(p+1) = 0x00
		ret				!   return
mzTry1:		cmp	r2,1			! else if byteCount == 1
		bne	mzDone			! .
		storeb	r0,[r1]			!   *p = 0x00
mzDone:						! endif
		ret				! return



! 
! =====================  MemoryCopy  =====================
! 
! external MemoryCopy (destPtr: ptr to void, srcPtr: ptr to void, byteCount: int)
!
! This routine copies "byteCount" bytes of data from the memory area pointed
! to by "srcPtr" to the area pointed to by "destPtr".  The pointers and the
! byteCount do not have to be multiples of 4 and the count may be less than
! zero, in which case, nothing will be copied.  However, the memory areas must
! not overlap!  (This routine does not test for overlap and if they overlap, the
! wrong data may be copied.)
!
! Here is the algorithm:
!
!      if (destPtr asInteger % 4 == 0) && 
!         (srcPtr asInteger % 4 == 0)
!        while byteCount > 3
!          *(destPtr asPtrTo int) = *(srcPtr asPtrTo int)
!          destPtr = destPtr + 4
!          srcPtr = srcPtr + 4
!          byteCount = byteCount - 4
!        endWhile
!      endIf
!      while byteCount > 0
!        *(destPtr asPtrTo char) = *(srcPtr asPtrTo char)
!        destPtr = destPtr + 1
!        srcPtr = srcPtr + 1
!        byteCount = byteCount - 1
!      endWhile
!
! Register usage:
!   r1 = destPtr
!   r2 = srcPtr
!   r3 = byteCount
!   r4 = work reg
! 
MemoryCopy:
		load	[r15+4],r1		! r1 = arg1 (destPtr)
		load	[r15+8],r2		! r2 = arg2 (srcPtr)
		load	[r15+12],r3		! r3 = arg3 (byteCount)
		and	r1,0x00000003,r4	! if destPtr % 4 == 0
		bne	mcDoBytes		! .
		and	r2,0x00000003,r4	! .   and srcPtr % 4 == 0
		bne	mcDoBytes		! .
mcWordLoop:					!   LOOP:
		cmp	r3,3			!     if byteCount <= 3 exit loop
		ble	mcWordLoopExit		!     .
		pop	[r2++],r4		!     *destPtr = *(srcPtr++)
		store	r4,[r1]			!     .
		add	r1,4,r1			!     destPtr = destPtr + 4
		sub	r3,4,r3			!     byteCount = byteCount - 4
		jmp	mcWordLoop		!   ENDLOOP
mcWordLoopExit:					!   .
mcDoBytes:					! endif
mcByteLoop:					! LOOP
		cmp	r3,0			!   if byteCount <= 0 exit loop
		ble	mcByteLoopExit		!   .
		loadb	[r2],r4			!   *destPtr = *srcPtr
		storeb	r4,[r1]			!   .
		add	r1,1,r1			!   destPtr = destPtr + 1
		add	r2,1,r2			!   srcPtr = srcPtr + 1
		sub	r3,1,r3			!   byteCount = byteCount - 1
		jmp	mcByteLoop		! ENDLOOP
mcByteLoopExit:					! .
		ret				! return

!
! =====================  GetCh  =====================
!
! This routine reads a character form input, and returns the ASCII
! value of the character received as an int.
!
! r2 = character
! r3 = ptr to SERIAL_STAT word in the memory-mapped area.
! r4 = ptr to SERIAL_DATA word in the memory-mapped area.
! r5 = serial status word
!

STACK_START     =       0x00ffff00
SERIAL_STAT     =       0x00ffff00
SERIAL_DATA     =       0x00ffff04

GetCh:
 		set SERIAL_STAT,r3 ! Copied from Echo.s
 		set SERIAL_DATA,r4 ! Copied from Echo.s
fetch:
		load [r3],r5 ! Fetch SERIAL_STAT(US)
		btst 0x00000001, r5 ! Check if status has char available
		be fetch ! If not, loop back to fetch
		load [r4],r1
		store r1,[r15+4] ! Load the SERIAL_DATA char
		ret

! 
! =====================  print  =====================
! 
! external print (s: ptr to array of char)
!
! This routine prints data directly and immediately to the output.  It is
! intended only for use in debugging the kernel; as such it bypass the serial
! I/O device entirely and uses the debug2 "back-door" to the virtual machine.
! It may be called from any code, including from within an interrupt handler.
!
! This routine will print \n, \t, and any printable ASCII characters.
! It will print \n as "\r\n".  It will print everything else in the form "\xHH".
! 
print:
		load	[r15+4],r2		! Move the argument "s" into r2
		pop	[r2++],r3		! Move the count into r3 and incr ptr
		mov	2,r1			! Move function code into r1
		debug2				! Do the upcall
		ret				! Return



! 
! =====================  printInt  =====================
! 
! external printInt (i: int)
!
! This routine prints data directly and immediately to the output.  It is
! intended only for use in debugging the kernel.
! 
printInt:
		load	[r15+4],r2		! Move the argument "i" into r2
		mov	1,r1			! Move function code into r1
		debug2				! Do the upcall
		ret				! Return



! 
! =====================  printHex  =====================
! 
! external printHex (i: int)
!
! This routine prints data directly and immediately to the output.  It is
! intended only for use in debugging the kernel.
!
! This routine will print the argument in the form "0x0012ABCD".
! 
printHex:
		load	[r15+4],r2		! Move the argument "i" into r2
		mov	6,r1			! Move function code into r1
		debug2				! Do the upcall
		ret				! Return



! 
! =====================  printChar  =====================
! 
! external printChar (c: char)
!
! This routine prints data directly and immediately to the output.  It is
! intended only for use in debugging the kernel.
!
! This routine will print \n, \t, and any printable ASCII character.
! It will print \n as "\r\n".  It will print everything else in the form "\xHH".
! 
printChar:
		loadb	[r15+4],r2		! Move the argument "c" into r2
		mov	3,r1			! Move function code into r1
		debug2				! Do the upcall
		ret				! Return



! 
! =====================  printBool  =====================
! 
! external printBool (b: bool)
!
! This routine prints data directly and immediately to the output.  It is
! intended only for use in debugging the kernel.
!
! This routine will print either "TRUE" or "FALSE".
! 
printBool:
		loadb	[r15+4],r2		! Move the argument "b" into r2
		mov	5,r1			! Move function code into r1
		debug2				! Do the upcall
		ret				! Return



! 
! =====================  printDouble  =====================
! 
! external printDouble (d: double)
!
! This routine prints data directly and immediately to the output.  It is
! intended only for use in debugging the kernel.
!
! Thris routine will print the argument in one of these forms:
!       1.23456e+22
!       Not-a-number
!       Positive-Infinity
!       Negative-Infinity
!       0
!       Negative-Zero
! 
printDouble:
		fload	[r15+4],f0		! Move the argument "d" into f0
		mov	4,r1			! Move function code into r1
		debug2				! Do the upcall
		ret				! Return



! 
! =====================  _heapInitialize  =====================
! 
! This routine is passed nothing and returns nothing.  It is called during
! startup to initialize the heap, as necessary.
!
! This routine will perform an "upcall" to a routine written in KPL.
!
! Registers modified: Same as any KPL function.
!
_heapInitialize:
	jmp	_P_System_KPLMemoryInitialize



! 
! =====================  _heapAlloc  =====================
! 
! This routine is passed the number of bytes in r1.  It allocates that many bytes
! and returns a pointer to the area in r1.  It never returns NULL.  If there is
! insufficient memory, an error will be signalled.
!
! This routine is called from code generated by the compiler.  It will perform
! an "upcall" to a routine written in KPL.
!
! Registers modified: Same as any KPL function.
!
_heapAlloc:
	push	r1				! Prepare the argument
	call	_P_System_KPLMemoryAlloc	! Perform the upcall
	pop	r1				! Retrieve the result
	ret



! 
! =====================  _heapFree  =====================
! 
! This routine is passed a pointer in r1.  This should point to a block of
! memory previously allocated using "malloc" (not "alloc").  It returns
! this memory to the free pool for subsequent allocation.
!
! This routine is called from code generated by the compiler.  It will perform
! an "upcall" to a routine written in KPL.
!
! Registers modified: Same as any KPL function.
!
_heapFree:
	push	r1				! Prepare the argument
	call	_P_System_KPLMemoryFree		! Perform the upcall
	pop	r1				! Pop the argument
	ret



! 
! =====================  _IsKindOf  =====================
! 
! This routine is passed a pointer to an object in r1 and a pointer to
! a type descriptor in r2.  It determines whether that object "is a kind of"
! that type.  It returns either TRUE (0x00000001) or FALSE (0x00000000) in r1.
!
! This routine is called from code generated by the compiler.  It will perform
! an "upcall" to a routine written in KPL.
!
! Registers modified: Same as any KPL function.
!
_IsKindOf:
	push	r2				! Push arg 2
	push	r1				! Push arg 1
	call	_P_System_KPLIsKindOf		! Perform the upcall
	pop	r1				! Pop result
	pop	r2				! Pop arg 2
	ret



! 
! =====================  _RestoreCatchStack  =====================
! 
! This routine is passed a pointer to a CatchRecord in r4 (possibly NULL).
! This is the previous value of "CatchStackTop".  This routine pops the
! CatchStack (calling _heapFree for each record) until the top thing on
! the CatchStack is the record pointed to by r4.
!
! Registers modified: Same as any KPL function.
!
! This routine is called from code generated by the compiler.  It does this:
!
!    <<  load   <temp>, r4    THIS IS DONE IN THE COMPILED CODE >>
!        r1 := r12 (CatchStack top ptr)
!        r12 := r4
!    loop:
!        if r1 == r4 goto done
!        if r1 == NULL goto _runtimeErrorRestoreCatchStackError
!                             This would occur if we are asked to pop
!                             the catch stack back to an earlier
!                             state, but after popping records, we never
!                             can get to the earlier state.  This might
!                             happen if there is a compiler logic error
!                             or if memory has gotten corrupted.
!        load   [r1], r2
!        push   r2
!        push   r4
!        free   (r1)
!        pop    r4
!        pop    r1
!        goto   loop
!    done:
!
_RestoreCatchStack:
	mov	r12,r1				! r1 = saved value of CatchStack top
	mov	r4,r12				! Save r4 as new CatchStack top
_RestoreCatchStack_loop:			! LOOP:
	cmp	r1,r4				!   if r1 == r4 goto DONE
	be	_RestoreCatchStack_done		!   .
	cmp	r1,0				!   if r1 == NULL goto error
	be	_runtimeErrorRestoreCatchStackError  !
	load	[r1], r2			!   r2 = r1->next 
	push	r2				!   save ptr to next record
	push	r4				!   save target ptr (r4)
	call	_heapFree			!   free the record pointed to by r1
	pop	r4				!   restore target ptr (r4)
	pop	r1				!   r1 = ptr to next record
	jmp	_RestoreCatchStack_loop		!   goto LOOP
_RestoreCatchStack_done:			! DONE:
	ret					!   return



! 
! =====================  _PerformThrow  =====================
! 
! This routine is passed r4 = an ErrorID (just a ptr to an error name string).
! It looks down the Catch Stack until it finds a CATCH_RECORD with a matching
! ErrorID.  Then it jumps to the corresponding catch code, using the ptr stored
! in the CATCH_RECORD.  It also restores the FP and SP to their saved values,
! which makes it possible to re-enter the suspended routine.
!
! This routine does not free any CATCH_RECORDS.
!
! When an error is thown, but not caught, this code will perform an upcall to
! the routine "KPLUncaughtThrow" in package "System".  That routine
! should print some info about the situation and then throw "UncaughtError".
! Thus, we should never be returning from the call.  (The user code may or may
! not catch "UncaughtError", but if this too is uncaught, this code will goto
! "_runtimeErrorFatalThrowError".)
!
! Registers modified: Same as any KPL function.
!
! This routine is called from code generated by the compiler.  It does this:
!
!     << r4 = errorID    THIS IS DONE IN THE COMPILED CODE >>
!        r1 = r12 (ptr to record on top of CATCH STACK)
!    loop:
!          if r1 == NULL
!            if r4 == "uncaughtException" goto _runtimeErrorFatalThrowError
!            call KPLUncaughtThrow
!            goto _runtimeErrorThrowHandlerHasReturned
!          end
!          if r1->errorID == r4 then break
!          r1 = r1->next
!        goto loop
!        restore FP from r1->oldFP
!        restore SP from r1->oldSP
!        jump to r1->catchCode
!
_PerformThrow:
	mov	r12,r1				! r1 = r12 (CatchStack top ptr)
_PerformThrow_loop:				! LOOP:
	cmp	r1,0				!   if r1 == NULL
	bne	_PerformThrow_else		!   .
	set	_Error_P_System_UncaughtThrowError,r2 ! if r4 == "UncaughtThrowError"
	cmp	r2,r4				!       .				
	be	_runtimeErrorFatalThrowError	!       goto runtime error
	load	[r14+-8],r1			!     Push ptr to current rout. desc.
	push	r1				!     .
	push	r13				!     Push the current line number
	push	r4				!     Push ptr to error name
	call	_P_System_KPLUncaughtThrow	!     call KPLUncaughtThrow
	pop	r4				!     retore regs
	pop	r13				!     .
	pop	r1				!     .
	jmp	_runtimeErrorThrowHandlerHasReturned  ! goto runtime error
_PerformThrow_else:				!   end
	load	[r1+4],r3			!   r3 = errorID of this record
	cmp	r3,r4				!   if it matches r4
	be	_PerformThrow_found		!     goto FOUND
	load	[r1],r1				!   r1 = r1->next
        jmp     _PerformThrow_loop		!   goto LOOP
_PerformThrow_found:				! FOUND:
	load	[r1+8],r2			!   r2 = catch code ptr
	load	[r1+12],r14			!   Restore FP
	load	[r1+16],r6			!   Save the new SP in r6
	jmp	r2				!   jump to catch code

