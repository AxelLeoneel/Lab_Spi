/*
 * SPI_Master.c
 *
 * Created: 24/07/2025 23:28:46
 * Author : axell
 */ 

/********************************************************************/
/*--------------------- Header (Libraries)--------------------------*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "SPI_Master/UARTlib.h"
#include "SPI_Master/SPI_lib.h"

/********************************************************************/



/********************************************************************/
/*-----------------------Function Prototypes------------------------*/
void setup();
void I_O();
void program();
void refresh(uint8_t val);

/********************************************************************/



/********************************************************************/
/*-----------------------Variable Declaration-----------------------*/
uint8_t adcval7 = 0x00; // Stores the ADCH7 (slave) value
uint8_t adcval6 = 0x00; // Stores the ADCH6 (slave) value
char stringADC6[16];
char stringADC7[16];

/********************************************************************/



/********************************************************************/
/*------------------------------Main--------------------------------*/
int main(void)
{
	// Global configuration of uC
	setup();
	// Instructions
	writeString("ADC values:");
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
	// UART configuration
	initUART();
	// I_O configuration
	I_O();
	// SPI configuration
	initSPI(SPI_MASTER_OSC_DIV16, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_LAST_EDGE);
	// Enable global interruption
	sei();
}

void I_O()
{
	// 8 bit counter (only 6 bits in PORTC)
	DDRC = 0xFF; // OUT
	PORTC = 0x00; // LOW
	// Additional 2 bits (in PORTD)
	DDRD |= (1 << DDD3) | (1 << DDD2); // OUT
	PORTD &= ~((1 << DDD3) | (1 << DDD2)); // LOW
	// Slave Select pin OUT
	DDRB |= (1 << DDB2);
	// Turn off Slave Select
	PORTB |= (1 << PORTB2);
}

void program()
{
	// Slave select
	PORTB &= ~(1 << PORTB2);
	// Sync delay
	_delay_ms(1);
	// Send command for SECOND byte
	spiWrite(0x0B);
	// Store the received data in variable
	adcval7 = spiRead();
	// Sync delay
	_delay_ms(10);
	// Send command for FIRST byte
	spiWrite(0x0A);
	// Store the received data in variable
	adcval6 = spiRead();
	
	// Show the ADC7 value in counter
	PORTC = adcval7 & 0b00111111;
	// Make use of provisional bits
	if (adcval7 & (1 << 6)) // 0bXXXX XXXX & 0b0100 0000 = 0b0X00 0000
	{
		PORTD |= (1 << PORTD2);
		} else {
		PORTD &= ~(1 << PORTD2);
	}
	if (adcval7 & (1 << 7))
	{
		PORTD |= (1 << PORTD3);
		} else {
		PORTD &= ~(1 << PORTD3);
	}
	// Undo slave select
	PORTB |= (1 << PORTB2);
	// Convert values to strings and write them on terminal
	writeString("ADC 6: ");
	itoa(adcval6, stringADC6, 10); // itoa (integer variable, buffer, decimal system)
	writeString(stringADC6);
	writeString("	ADC 7: ");
	itoa(adcval7, stringADC7, 10); // itoa (integer variable, buffer, decimal system)
	writeString(stringADC7);
	writeChar('\n');
	// Anti sat delay
	_delay_ms(100);
}

/********************************************************************/



/********************************************************************/
/*----------------------Interrupt Subroutines-----------------------*/
/*ISR(USART_RX_vect)
{
	// Set flag
	recieve = 1;
	// Store the UDR0 in a char
	temporal = UDR0;
	writeChar('\n');
}*/



/********************************************************************/




