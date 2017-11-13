; This program adds together two 64 bit numbers
; and produces a 64 bit result
	AREA DATA
; define the array length (similar to #define in C)

HighSum   DCD 0
LowSum	  DCD 0
Stack  SPACE 0x100 	; allocate some space for the stack
	
	; The following symols are in the CODE section (ROM,Executable (Thumb), readonly)	
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

; Define const data: Note: Little endian
Operand1	DCD	0x9abcdef0,0x12345678
Operand2	DCD	0xabcdef01,0x23456789

	; 'Main' program goes here
Reset_Handler
		
		LDR R5,=Operand1
		LDR R6,=Operand2
		LDR R0,[R5]
		LDR R2,[R6]
		ADDS R0,R0,R2
		LDR R5,=Operand1+4
		LDR R6,=Operand2+4
		LDR R1,[R5]
		LDR R2,[R6]
		ADCS R1,R1,R2
		LDR R5,=LowSum
		STR R0,[R5]
		LDR R5,=HighSum
		STR R1,[R5]
		
stop	B stop		
	end
