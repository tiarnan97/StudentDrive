; This program demonstrates the use of
; subroutines
	AREA DATA

TargetString SPACE 20
	
	; The following symols are in the CODE section (ROM,Executable (Thumb), readonly)	
	AREA THUMB,CODE,READONLY
	; EXPORTED Symbols can be linked against	
		EXPORT Reset_Handler
		EXPORT __Vectors
	; Minimal interrupt vector table follows
	; First entry is initial stack pointer (end of stack)
	; second entry is the address of the reset handler
__Vectors
		DCD 0x20001000
		DCD Reset_Handler 
	; 'Main' program goes here
Reset_Handler

		MOVS R0,#12
		BL square
		LDR R1,=TestString
		BL strlen		
		LDR R1,=TargetString
		LDR R2,=TestString
		BL strncpy
stop	B stop		
; functions
; int square(int)
; This function takes a single argument in R0,
; squares it and returns to the caller
; parameter is passed via R0 and result is returned
; in R0
square
	push {LR}
	muls R0,R0,R0
	pop {PC}
; int strlen(char *str)
; This function takes a single argument: a pointer
; to a string in R1.  It searches through the
; string looking for a terminating null, counting
; as it goes.  This count is returned as the
; length of the string to the caller via R0.
;
strlen
	push {LR}		; save any registers that are
	push {R1}
	push {R2}		; changed (apart from R0)
	MOVS R0,#0   	; zero the counter
	MOVS R2,#0		; clear out R2
strlen_loop
	LDRB R2,[R1] 	; read a character
	CMP R2,#0		;
	BEQ	strlen_exit
	ADDS R0,R0,#1   ; increment count
	ADDS R1,R1,#1	; increment pointer
	B strlen_loop	; back to top of loop
strlen_exit
	pop {R2}
	pop {R1}
	pop {PC}		
	
; int strcpy(char *dst, char *src)
; This function takes two arguments:
; a source string (src) and a destination
; string (dst).  The function copies the 
; contents of src to dst.  All bytes
; up to and including the terminating
; null (hopefully present) will be copied
; On entry R1 points to the destination string
; R2 points to the source string.  On 
; exit R0 contains a count of the bytes copied
strncpy
	push {LR,R1-R3}

	movs R0,#0	 	; zero the count
	movs R3,#0		; clear out R3
	
strcpy
	push {LR,R1-R3}

	movs R0,#0	 	; zero the count
	movs R3,#0		; clear out R3
	
strncpy_loop
	ldrb R3,[R2] 	; read byte
	strb R3,[R1]	; write byte
	adds R0,R0,#1	; increment count
	adds R1,#1		; increment dst
	adds R2,#1		; increment src
	cmp R3,#0		; at the end
	beq strncpy_exit	; if so, exit
	b strncpy_loop	; else go back
strncpy_exit		
	pop {PC,R1-R3}
TestString DCB "abcdef",0
	end
