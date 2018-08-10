/**
 *
 * Blinking LED ATTiny85 "hello world"
 *
 * Following the tutorial at:
 * http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/?ALLSTEPS
 *
 */

#include <avr/io.h>
// F_CPU frequency to be defined at command line
#include <util/delay.h>

// LED is on pin 2, PB3
#define LED      PB3
#define DELAY_MS 1000

// write digital "high" to pin <pn> on port <prt>
#define DIGIWRITE_H(prt, pn) prt |= (1<<pn)

// write digital "low" to pin <pn> on port <prt>
#define DIGIWRITE_L(prt, pn) prt &= ~(1<<pn)

// Define long delay function
// uses a for loop to get around the requirement that the avr built-in
// underlying _delay_ms requires compile time constant
void long_delay_ms(uint16_t ms) {
    for(ms /= 10; ms>0; ms--) _delay_ms(10); 	// initializes with /= bc it's
																							// the number of 10s of ms to run
																							// and the code block is _delay_ms(10)
}


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
