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
uint8_t UARTvalue = 0; // Stores the UART (computer-master) value sent by SPI

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
	//UCSR0B = 0x00;
	// I_O configuration
	I_O();
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
	PORTD = UARTvalue;
	_delay_ms(100);
}

/********************************************************************/



/********************************************************************/
/*----------------------Interrupt Subroutines-----------------------*/
ISR(SPI_STC_vect)
{
	UARTvalue = SPDR;
}

/********************************************************************/


