	; This is a comment
	; Symbols start up against the left hand margin
	; The following symbols reside in RAM
	AREA DATA
	
Result DCD 0		; allocate space for the result of a calculion
Stack  SPACE 0x100 	; allocate some space for the stack
	
	; The following symols are in the CODE section (ROM,Executable, readonly)	
	AREA THUMB,CODE,READONLY
	; EXPORTED Symbols can be linked against	
		EXPORT Reset_Handler
		EXPORT __Vectors
	; Minimal interrupt vector table follows
	; First entry is initial stack pointer (end of stack)
	; second entry is the address of the reset handler
__Vectors
		DCD Stack+0x100		
		DCD Reset_Handler 
	; Define some constant operands
Num1   DCD 0xfffffffa
Num2   DCD 8
	; 'Main' program goes here
Reset_Handler
		LDR R1,=Num1
		LDR R2,=Num2
		LDR R0,[R1]
		LDR R1,[R2]
		; The instruction to subract is SUBS
		ADDS R0,R0,R1
		LDR R1,=Result
		STR R0,[R1]
stop	B stop		
	end