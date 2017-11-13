/* Blinky with a button.
	An LED is attached to Pin 25 and a button is attached to Pin 1
  When the button is pressed, the LED should blink

*/

#include "lpc111x.h"

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

int main()
{	
	
	ConfigPins();
	
	while(1) 
	{
		if (GPIO0DATA & BIT8)
		{
			GPIO0DATA ^= BIT2;
			delay(1000000);
		}
	
	}    
}



