/* This program uses UART to send data back to the host using
 * simple print statements.  UART transmission is not interrupt
 * driven.  Reception is not implemented in this example.
 * 
 * Operation:
 * 
 * Wiring.
 * A USB to serial converter is wired to the TX and RX pins of UART0
 */
#include "lpc111x.h"
void SysTick(void){}

void ConfigPins()
{
	SYSAHBCLKCTRL |= BIT6 + BIT13 + BIT16; // Turn on clock for GPIO, ADC and IOCON 
		
	IOCON_PIO0_2 &= ~(BIT1+BIT0); // ensure Pin 25 behaves as GPIO
	GPIO0DIR |= BIT2;	// Make Pin 25 an output

	// select analog mode for PIO1_0
	IOCON_R_PIO1_0 = 2;
	// Power up the ADC
	PDRUNCFG &= ~BIT4; 
	// set ADC clock rate and select channel 1
	AD0CR=(11<<8)+BIT1;
}
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
	U0LCR &= ~BIT7; // Disable divisor latch access
	U0LCR |= (BIT1+BIT0); // set word lenght to 8 bits.
	
}
void delay(int dly)
{
  while (dly--);
}

void eputc(char c)
{
	U0THR = c;
	while((U0LSR & BIT5) == 0); // Wait for tx to finish
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
void printInteger(unsigned int Number)
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

int ReadADC()
{
	// Start a conversion
	AD0CR |=BIT1+BIT24;
	// Wait for conversion to complete
	while((AD0GDR&BIT31)==0);
	// return the result
	return ((AD0GDR>>6)&0x3ff);
}


int main() 
{	
	int input;
	initUART();
	ConfigPins();
	int Num=0x0;
	printString("Hello world\r\n");
	
	while(1) 
	{		
		input=ReadADC();
		printString("The value of ADC Input is "); 
		printInteger(input);
		printString("\r\n");
		delay(100000);
		//Num++;
	}    
}



