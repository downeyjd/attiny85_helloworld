/**
 *
 * working up to a nRF24L01+ controller to transmit built-in temp sensor data
 *
 * Following the tutorial at:
 * http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/?ALLSTEPS
 * and Jack Christensen's tinySPI library
 * https://github.com/JChristensen/tinySPI
 *
 * #define SPI_DDR_PORT 	DDRB
 * #define USCK_DD_PIN 		PB2
 * #define DO_DD_PIN		PB1 
 * #define DI_DD_PIN 		PB0
*/

#ifndef __AVR_ATtiny85__ // added bc IDEs don't get compiler flag for ATtiny85 and header files need that info
#define __AVR_ATtiny85__
#endif

#include <avr/io.h>  // F_CPU frequency to be defined at command line, using 8e6 (8MHz)
#include <util/delay.h>
#include "tinySPI.h"

#define LED_PIN PB3
#define DELAY_MS 1000

#define DIGIWRITE_H(prt, pn) prt |= (1 << pn)
#define DIGIWRITE_L(prt, pn) prt &= ~(1 << pn)

// Define long delay function
// uses a for loop to get around the requirement that the avr built-in
// underlying _delay_ms requires compile time constant
void long_delay_ms(uint16_t ms)
{
	for (ms /= 10; ms > 0; ms--)
		_delay_ms(10); // initializes with /= bc it's
					   // the number of 10s of ms to run
					   // and the code block is _delay_ms(10)
}

int main()
{
	uint8_t high = 0;
	uint16_t ms = 0;

	// setup LED pin for output in port B's direction register
	DDRB |= (1 << LED_PIN);

	// set LED pin LOW
	//PORTB &= ~(1 << LED);
	DIGIWRITE_L(PORTB, LED_PIN);

	SPIbegin();
	SPIsetDataMode(SPI_MODE0);


	while (1)
	{
		high = !high;

		if (high)
		{
			// set LED pin HIGH
			DIGIWRITE_H(PORTB, LED_PIN);
		}
		else
		{
			// set LED pin LOW
			DIGIWRITE_L(PORTB, LED_PIN);
		}

		// delay for 500 ms
		for (ms = DELAY_MS; ms > 0; ms -= 10)
		{
			_delay_ms(10);
		}
	}

	return 0;
}