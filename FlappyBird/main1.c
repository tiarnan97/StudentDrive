/* 
 */

#include "lpc111x.h"

#define SCREEN_WIDTH 80
void initSysTick(void);
unsigned prbs(void);
int Tunnel;
int Width=20;
int Health=16;

void SysTick(void)
{
	static int milliseconds=0;	
	milliseconds++;
	if (milliseconds >=1000)
	{
		GPIO0DATA ^= BIT2;
		milliseconds=0;
	}
	prbs(); // stir the random number pot
}
void initSysTick()
{
	
	// The systick timer is driven by a 48MHz clock
	// Divide this down to achieve a 1ms timebase
	// Divisor = 48MHz/1000Hz
	// Reload value = 48000-1
	// enable systick and its interrupts
	SYST_CSR |=(BIT0+BIT1+BIT2); 
	SYST_RVR=48000-1; // generate 1 second time base
	SYST_CVR=5;
	enable_interrupts();
}
void delay(int dly)
{
  while (dly--);
}
unsigned prbs()
{
	// This is an unverified 31 bit PRBS generator
	// It should be maximum length but this has not been verified 
	static unsigned long shift_register=0xa5a5a5a5;
	unsigned long new_bit=0;
	static int busy=0; // need to prevent re-entrancy here
	if (!busy)
	{
		busy=1;
		new_bit= ((shift_register & (1<<27))>>27) ^ ((shift_register & (1<<30))>>30);
		new_bit= ~new_bit;
		new_bit = new_bit & 1;
		shift_register=shift_register << 1;
		shift_register=shift_register | (new_bit);
		busy=0;
	}
	return shift_register & 0x7ffffff; // return 31 LSB's 
}
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
	U0LCR |= (BIT1+BIT0); // set word length to 8 bits.
	
}
void eputc(char c)
{
	U0THR = c;
	while((U0LSR & BIT5) == 0); // Wait for tx to finish
}
char egetc()
{

	if (U0LSR & BIT0) // RX Byte available
	{
		return U0RBR;
	}
	else
		return 0;
		
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
void updateScreen(int Position)
{
	int x=0;
	Position=Position/10;
	while(x < SCREEN_WIDTH)
	{	
		if (x<=(Tunnel-Width/2))
			eputc('.');
		else
		if (x>=(Tunnel+Width/2))
			eputc('.');
		else
		if (x==Position)
			eputc('@');
		else
			eputc(' ');		
		x++;
	}
	if ( (Position <=(Tunnel-Width/2)) || (Position >=(Tunnel+Width/2)))
	{
		Health--;
		if (Health==0)
		{
			printString("\r\nGame Over Loser!");
			printString("\r\nPress RESET to restart");
			while(1);
		}
	}
	printInteger(Health);
	printString("\r\n");
	
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
	initSysTick();
	initUART();
	ConfigPins();
	printString("\r\nWelcome to ADC Runner\r\n");	
	printString("Centre the controller and press any key to start");
	//while(1);
	while(!egetc());	
	input=ReadADC()/10;
	Tunnel=input; // centre the on the player to begin with.
	while(1) 
	{	
		input=ReadADC();
		//printInteger(input);
		updateScreen(input);	
		if (prbs()&1)
			Tunnel++;
		else
			Tunnel--;			
		if (Tunnel < Width/2)
			Tunnel=1+Width/2;
		if (Tunnel > (SCREEN_WIDTH-1-Width/2))
			Tunnel=(SCREEN_WIDTH-1)-Width/2;
	}    
}



