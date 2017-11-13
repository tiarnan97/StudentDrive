#include "lpc111x.h"
void initUART()
{
	SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON
	// Enable UART RX function on PIO1_6
	IOCON_PIO1_6 |= BIT0; 
	IOCON_PIO1_6 &= ~(BIT1+BIT2);
	// Enable UART TX function on PIO1_7
	IOCON_PIO1_7 |= BIT0;
	IOCON_PIO1_7 &= ~(BIT1+BIT2);
	// Turn on clock for UART
	SYSAHBCLKCTRL |= BIT12;
	UARTCLKDIV = 1;
	// PCLK = 48Mhz. Desired Baud rate = 9600
	// See table 199
	// 9600=48MHz/(16* (256*U0DLM + U0DLL)*(1+DivAddVal/MulVal))
	// 312.5 = (256*U0DLM+U0DLL)*(1+DivAddVal/MulVal)
	// let U0DLM=1, DivAddVal=0,MulVal =1
	// 312.5=256+U0DLL
	// U0DLL=56.5.  
	// Choose U0DLL=56.
	// Actual baud rate achieved = 9615 - close enough.
	U0LCR |= BIT7; // Enable divisor latch access
	U0FDR = (1<<4)+0; // Set DivAddVal = 0; MulVal = 1
	U0DLL = 56;
	U0DLM = 1;
  U0LCR |= (BIT1+BIT0); // set word length to 8 bits.
	U0FCR = (BIT0 | BIT1 | BIT2); // Turn on FIFO, reset them and set trigger to 1 byte
	U0LCR &= ~BIT7; // Disable divisor latch access		
}
void eputc(char c)
{
	U0THR = c;
	while((U0LSR & BIT5) == 0); // Wait for tx to finish
}
char egetc()
{
	return U0RBR;	
}
void printString(char *String)
{
	while(*String)
	{
		eputc(*String);
		String++;
	}
}
char HexDigit(int Value)
{
	if ((Value >=0) && (Value < 10))
		return Value+'0';
	else if ((Value >9) && (Value < 16))
		return Value-10 + 'A';
	else
		return 'z';
}
void printHex(unsigned int Number)
{
	// Output the number over the serial port as
	// as hexadecimal string.
	char TxString[9];
	int Index=8;
	TxString[Index]=0; // terminate the string
	Index--;
	while(Index >=0)
	{
		TxString[Index]=HexDigit(Number & 0x0f);
		Number = Number >> 4;
		Index--;
	}
	printString(TxString);
}
