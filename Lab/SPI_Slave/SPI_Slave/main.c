/*
 * SPI_Slave.c
 *
 * Created: 24/07/2025 22:51:31
 * Author : axell
 */ 

/********************************************************************/
/*--------------------- Header (Libraries)--------------------------*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SPI_Slave/ADClib.h"
#include "SPI_Slave/SPI_lib.h"

/********************************************************************/



/********************************************************************/
/*-----------------------Function Prototypes------------------------*/
void setup();
void I_O();
void program();

/********************************************************************/



/********************************************************************/
/*-----------------------Variable Declaration-----------------------*/
uint8_t channel = 6; // Starts the ADC channel in ADC6
uint8_t adcval6 = 0; // Stores the ADCH6 value
uint8_t adcval7 = 0; // Stores the ADCH7 value

/********************************************************************/



/********************************************************************/
/*------------------------------Main--------------------------------*/
int main(void)
{
	// Global configuration of uC
	setup();
	// Main loop
	while (1)
	{
		program();
	}
}

/********************************************************************/



/********************************************************************/
/*---------------------Non-interrupt Subroutines--------------------*/
void setup()
{
	// Disable global interruption
	cli();
	// Use 16MHz as F_cpu
	CLKPR = (1 << CLKPCE);
	CLKPR = 0x00;
	// Disable serial communication
	UCSR0B = 0x00;
	// I_O configuration
	I_O();
	// ADC configuration
	configADC(channel);
	// SPI configuration
	initSPI(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_LAST_EDGE);
	// SPI interruption enable
	SPCR |= (1 << SPIE);
	// Enable global interruption
	sei();
}

void I_O()
{
	// 8 bit counter
	DDRD = 0xFF; // OUT
	PORTD = 0x00; // LOW
}

void program()
{
	// Display ADC6 value on PORTD
	PORTD = adcval6;
	_delay_ms(100);
}

/********************************************************************/



/********************************************************************/
/*----------------------Interrupt Subroutines-----------------------*/
ISR(ADC_vect)
{
	cli();
	// Alternate between ADC channels
	if (channel == 6)
	{
		// Store the current ADC value in variable
		adcval6 = ADCH;
		// Change channel
		channel = 7;
	}
	else
	{
		// Store the current ADC value in variable
		adcval7 = ADCH;
		// Change channel
		channel = 6;
	}
	// Configure next channel and start conversion
	configChannel(channel);
	ADCSRA |= (1 << ADSC);
	sei();
}

ISR(SPI_STC_vect)
{
	uint8_t spival = SPDR;
	if (spival == 0x0A)
	{
		spiWrite(adcval6);
	}
	else if (spival == 0x0B)
	{
		spiWrite(adcval7);
	}
}

/********************************************************************/


