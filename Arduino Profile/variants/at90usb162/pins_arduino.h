/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// Workaround for wrong definitions in "iom32u4.h".
// This should be fixed in the AVR toolchain.

#define NUM_DIGITAL_PINS  21
#define NUM_ANALOG_INPUTS 0

#define TX_RX_LED_INIT	//DDRD |= (1<<0)
#define TXLED0			//PORTD |= (1<<0)
#define TXLED1			//PORTD &= ~(1<<0)
#define RXLED0			//PORTD |= (1<<0)
#define RXLED1			//PORTD &= ~(1<<0)

#define PIN_WIRE_SDA         (2)
#define PIN_WIRE_SCL         (3)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 13
#define LED_BUILTIN_RX 13
#define LED_BUILTIN_TX 13

// Map SPI port to 'new' pins D14..D17
#define PIN_SPI_SS    (0)
#define PIN_SPI_MOSI  (2)
#define PIN_SPI_MISO  (3)
#define PIN_SPI_SCK   (1)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

// Mapping of analog pins as digital I/O
// A6-A11 share with digital pins

#define digitalPinToPCICR(p)    ( (((p) >= 0) && ((p) <= 7)) || ((p) == 18) || (((p) >= 9) && ((p) <= 12)) ? (&PCICR) : ((uint8_t *)0) )
#define digitalPinToPCICRbit(p) ( (((p) >= 9) && ((p) <= 12)) || ((p) == 18) ? 1 : 0 )
#define digitalPinToPCMSK(p)    ( (((p) >= 0) && ((p) <= 7)) ? (&PCMSK0) : ( (((p) >= 9) && ((p) <= 12)) || ((p) == 18) ? (&PCMSK1) : ((uint8_t *)0) ) )
#define digitalPinToPCMSKbit(p) ( (((p) >= 0) && ((p) <= 7)) ? (p) : ( ((p) == 18) ? 4 : ( (((p) >= 9) && ((p) <= 12)) ? ((p) - 9) : 0 ) ) )


#define digitalPinHasPWM(p)         ((p) == 7 || (p) == 9 || (p) == 10 || (p) == 13)

#define digitalPinToInterrupt(p) ((p) == 13 ? INT0 : ((p) == 14 ? INT1 : ((p) == 15 ? INT2 : ((p) == 16 ? INT3 : ((p) == 8 ? INT4 : ((p) == 17 ? INT5 : ((p) == 19 ? INT6 : ((p) == 20 ? INT7 : NOT_AN_INTERRUPT))))))))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA32U4 / ARDUINO LEONARDO
//
// D0				PD2					RXD1/INT2
// D1				PD3					TXD1/INT3
// D2				PD1		SDA			SDA/INT1
// D3#				PD0		PWM8/SCL	OC0B/SCL/INT0
// D4		A6		PD4					ADC8
// D5#				PC6		???			OC3A/#OC4A
// D6#		A7		PD7		FastPWM		#OC4D/ADC10
// D7				PE6					INT6/AIN0
//
// D8		A8		PB4					ADC11/PCINT4
// D9#		A9		PB5		PWM16		OC1A/#OC4B/ADC12/PCINT5
// D10#		A10		PB6		PWM16		OC1B/0c4B/ADC13/PCINT6
// D11#				PB7		PWM8/16		0C0A/OC1C/#RTS/PCINT7
// D12		A11		PD6					T1/#OC4D/ADC9
// D13#				PC7		PWM10		CLK0/OC4A
//
// A0		D18		PF7					ADC7
// A1		D19		PF6					ADC6
// A2		D20 	PF5					ADC5
// A3		D21 	PF4					ADC4
// A4		D22		PF1					ADC1
// A5		D23 	PF0					ADC0
//
// New pins D14..D17 to map SPI port to digital pins
//
// MISO		D14		PB3					MISO,PCINT3
// SCK		D15		PB1					SCK,PCINT1
// MOSI		D16		PB2					MOSI,PCINT2
// SS		D17		PB0					RXLED,SS/PCINT0
//
// TXLED	D30		PD5					XCK1
// RXLED	D17	    PB0
// HWB				PE2					HWB

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM interrupt_num_to_INT_PGM[] = {
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	INT4,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	NOT_AN_INTERRUPT,
	INT0,
	INT1,
	INT2,
	INT3,
	INT5,
	NOT_AN_INTERRUPT,
	INT6,
	INT7,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
        PB, /* 0 */
        PB,
        PB,
        PB,
        PB,
        PB,
        PB,
        PB,
        PC, /* 8 */
        PC,
        PC,
        PC,
        PC,
        PD, /* 13 */
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
        _BV(0), /* 0, port B */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(7), /* 8, port C */
        _BV(6),
        _BV(5),
        _BV(4),
        _BV(2),
        _BV(0), /* 13, port D */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
NOT_ON_TIMER, /* 0 - port B */
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        TIMER1C,
        NOT_ON_TIMER, /* 8 - port C */
        TIMER1A,
        TIMER1B,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        TIMER0B, /* 13 - port D */
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
};

const uint8_t PROGMEM analog_pin_to_channel_PGM[] = {
	7,	// A0				PF7					ADC7
	6,	// A1				PF6					ADC6	
	5,	// A2				PF5					ADC5	
	4,	// A3				PF4					ADC4
	1,	// A4				PF1					ADC1	
	0,	// A5				PF0					ADC0	
	8,	// A6		D4		PD4					ADC8
	10,	// A7		D6		PD7					ADC10
	11,	// A8		D8		PB4					ADC11
	12,	// A9		D9		PB5					ADC12
	13,	// A10		D10		PB6					ADC13
	9	// A11		D12		PD6					ADC9
};

#endif /* ARDUINO_MAIN */

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR        Serial
#define SERIAL_PORT_USBVIRTUAL     Serial
#define SERIAL_PORT_HARDWARE       Serial1
#define SERIAL_PORT_HARDWARE_OPEN  Serial1

// Alias SerialUSB to Serial
#define SerialUSB SERIAL_PORT_USBVIRTUAL

// These are used by the core
#define USB_CONTROL_EP_SIZE		16
#define USB_CONTROL_EP_BANKS		1

#define USB_DEFAULT_EP_SIZE		32
#define USB_DEFAULT_EP_BANKS		2

#define USB_CDC_NOTIFICATION_EP_SIZE	16
#define USB_CDC_NOTIFICATION_EP_BANKS	1

#define USB_CDC_DATA_EP_SIZE		32
#define USB_CDC_DATA_EP_BANKS		2

// These can optionally be used by PluggableUSB libraries
#define USB_HID_EP_SIZE			16 
#define USB_HID_EP_BANKS		1

#define USB_MIDI_EP_SIZE		32
#define USB_MIDI_EP_BANKS		2

#define USB_MSD_EP_SIZE			32
#define USB_MSD_EP_BANKS		2

#define USB_EP_SIZE 16

#endif /* Pins_Arduino_h */
