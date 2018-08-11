
/**
 *  
 * Blinking LED ATTiny85 "hello world"
 *
 * Following the tutorial at:
 * http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/?ALLSTEPS
 *  
 */

#ifndef __AVR_ATtiny85__ // added bc IDEs don't get compiler flag for ATtiny85 and header files need that info
#define __AVR_ATtiny85__
#endif

#include <avr/io.h>
// F_CPU frequency to be defined at command line
#include <util/delay.h>

// LED is on pin 2, PB3
#define LED      PB3
#define DELAY_MS 1000

int main () {
	uint8_t high = 0;
	uint16_t ms = 0;

	// setup LED pin for output in port B's direction register
	DDRB |= (1 << LED);

	// set LED pin LOW
	PORTB &= ~(1 << LED);

	while (1) {	
		high = !high;

		if (high) {
			// set LED pin HIGH
			PORTB |= (1 << LED);
		} else {
			// set LED pin LOW
			PORTB &= ~(1 << LED);
		}

		// delay for 500 ms
		for (ms = DELAY_MS; ms > 0; ms -= 10) {
			_delay_ms(10);
		}
	}

	return 0;
}

