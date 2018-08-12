// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Arduino hardware SPI master library for
// ATtiny24/44/84, ATtiny25/45/85, ATtiny261/461/861, ATtiny2313/4313.
//
// https://github.com/JChristensen/tinySPI
// Jack Christensen 24Oct2013

#ifndef __AVR_ATtiny85__ // added bc IDEs don't get compiler flag for ATtiny85 and header files need that info
#define __AVR_ATtiny85__
#endif

#include <avr/io.h>
#include <util/atomic.h>

// USI ports and pins
#define SPI_DDR_PORT    DDRB
#define USCK_DD_PIN     PB2
#define DO_DD_PIN       PB1
#define DI_DD_PIN       PB0

// SPI data modes
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04

static void SPIbegin()
{
    USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1)); // sets USISIE (interrupt), USIOIE (interrupt), USIWM1 (part of Wire Mode, with USIWM0) in USICR to 0
    USICR |= _BV(USIWM0) | _BV(USICS1) | _BV(USICLK); // sets USMIWM0 (+USIWM1 = 3wire mode), USICS1 (part of Clock Source Select, with USICS0 makes external clock,
    // clock edge (+/-) set in USICS0 in setDataMode below), and USICLK (Counter Clock source = software clock strobe at USITC) in USICR to 1.
    SPI_DDR_PORT |= _BV(USCK_DD_PIN);   // set the USCK pin as output, 1 in DDR means output, 0 means input
    SPI_DDR_PORT |= _BV(DO_DD_PIN);     // set the DO pin as output , bitwise or (|=) of bitmask sets to 1
    SPI_DDR_PORT &= ~_BV(DI_DD_PIN);    // set the DI pin as input, bitwise and (&=) of inverse bitmask sets to 0
};

static void SPIsetDataMode(uint8_t spiDataMode)
{
    if (spiDataMode == SPI_MODE1)
        USICR |= _BV(USICS0);
    else
        USICR &= ~_BV(USICS0);            
};

static uint8_t SPItransfer(uint8_t spiData)
{
    USIDR = spiData;                    // load data to USI data register (which can contain income and outgoing data...?)
    USISR = _BV(USIOIF);                // clear counter and counter overflow interrupt flag (sets whole register to zero,
                                        // except USIOIF, the overflow flag, which must be set to 1 to clear)
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period, this temporary disables interrupts until tx/rx complete.
    {
        while ( !(USISR & _BV(USIOIF)) )    // bitwise & of the whole USI status register and the overflow flag bitmask, which
                                            // sets the whole register to zero, except USIOIF, which will only be 1 if overflow flag set
                                            // so while that flag isn't set... waits for transmission of 8 bits = 8 clock pulses = 16 edges
                                            // = full 4-bit counter = overflow = USIOIF -> 1!
        {
            USICR |= _BV(USITC);        // bitwise or the whole USI control register with USITC bitmask, which sets 1 to USITC, which toggles that line
                                        // creating the SPI master clock signal to drive transmission
        };
    };
    return USIDR;                       // now that the master and slave have completely swapped USIDRs, return the slave's data.
};

static void SPIend()
{
    USICR &= ~(_BV(USIWM1) | _BV(USIWM0)); // settings both wiremode bits in USICR to zero disables the USI.  Pins still available as standard IO pins
};