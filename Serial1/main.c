/* Serial Blinky.
	An LED is attached to Pin 25.
	A single character is polled using egetc.
	A switch statement is used to turn on the LED if the character is "L"
	and turn off the LED if the character is "M".
	All received characters are echoed to the terminal. The terminal must be enabled. 
	This is done in the Build.bat file using -term.
	
	A Button can button can be attached to Pin 1.
  
*/

#include "lpc111x.h"
void initUART()
{
	SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON
	// Enable UART RX function on PIO1_6
	IOCON_PIO1_6 |= BIT0; 
	//IOCON_PIO1_6 &= ~(BIT1+BIT2);
	// Enable UART TX function on PIO1_7
	IOCON_PIO1_7 |= BIT0;
	//IOCON_PIO1_7 &= ~(BIT1+BIT2);
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
void eputc(char c)
{
	U0THR = c;
	while((U0LSR & BIT5) == 0); // Wait for tx to finish
}
char egetc()
{
	while((U0LSR & BIT0) ==0); // wait for a byte to arrive
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

void delay(unsigned len)
{
	while(len--);
}
void ConfigPins()
{
	SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON 
	IOCON_PIO0_2 &= ~(BIT1+BIT0);  // ensure Pin 25 behaves as GPIO
	GPIO0DIR |= BIT2;	// Make Pin 25 an output
	GPIO0DIR &= ~BIT8;	// Make Pin 0 an input
	GPIO0DATA = 0;		// 0 output initially
}


void WaitForBtnPress()
{
while ( (GPIO0DATA & BIT1) != 0);
}
void WaitForBtnRelease()
{
while ((GPIO0DATA & BIT1)==0);
} 

int main()
{	
int state =0;

	
	ConfigPins();
	initUART();
	int rx_byte;
	printString("Press L to turn on the LED, M to turn off\r\n");
	while(1) 
	{
	
	
	WaitForBtnPress();
	WaitForBtnRelease();
	
	if (state == 0)
	{
		state = 1;
		eputc('M');
	}
	
	else {
		state = 0;
		eputc('L');
	}
	    rx_byte=egetc();  // read an incoming byte (waits for input)            
	    switch (rx_byte)
		{
			case  'M' : {
				GPIO0DATA |= BIT2;break; // Set port bit high
			}
			case 'L' : {
				GPIO0DATA &= ~BIT2;break; // Set port bit low
			}
		}	    
	    delay(100);	    
	    eputc(rx_byte);     //transmit switch_word
    
	
	}    
}



