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

/********************************************************************/



/********************************************************************/
/*-----------------------Variable Declaration-----------------------*/
char received; // Receives the current UART char
uint8_t UARTflag = 0; // Flag for control
uint8_t total = 0;

/********************************************************************/



/********************************************************************/
/*------------------------------Main--------------------------------*/
int main(void)
{
	// Global configuration of uC
	setup();
	// Boot sequence
	writeString("Welcome\n");
	// Show instructions
	writeString("Write a value between 0 and 255\n");
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
	if (UARTflag == 1)
	{
		// Clear the flag
		UARTflag = 0;
		/*// String to int conversion
		UARTval = atoi(UARTstring);
		// Verify range
		if (UARTval >= 256)
		{
			// If the value is Out of range, becomes zero
			writeString("Out of range\n");
			UARTval = 0;
		}
		// Write entered value in monitor
		//writeString(UARTstring);*/
		// Slave select
		PORTB &= ~(1 << PORTB2);
		_delay_ms(10);
		// Send UART value by SPI
		spiWrite(total);
		_delay_ms(10); 
		// Show the ADC7 value in counter
		PORTC = total & 0b00111111;
		// Make use of provisional bits
		if (total & (1 << 6)) // 0bXXXX XXXX & 0b0100 0000 = 0b0X00 0000
		{
			PORTD |= (1 << PORTD2);
			} else {
			PORTD &= ~(1 << PORTD2);
		}
		if (total & (1 << 7))
		{
			PORTD |= (1 << PORTD3);
			} else {
			PORTD &= ~(1 << PORTD3);
		}
		// Undo slave select
		PORTB |= (1 << PORTB2);
		total = 0;
		/*// Clear array
		for (uint8_t j = 0; j >=16; j++)
		{
			UARTstring[j] = '\0';
		}*/
		_delay_ms(100);
	}
	
}

/********************************************************************/



/********************************************************************/
/*----------------------Interrupt Subroutines-----------------------*/
ISR(USART_RX_vect)
{
	// Store the entered char
	/*received = UDR0;
	// If the char is end of line or the end of string
	if(received != '\n' || received != '\r')
	{
		// Save the char on index into the string
		UARTstring[i] = received;
		// Inc the index
		i++;
	}
	else
	{
		// Close the string
		UARTstring[i+1] = '\0';
	}*/
	received = UDR0;
	if (received >= '0' && received <= '9') {
		total = total * 10 + (received - '0');
	}
	else if (received == '\n' || received == '\r') {
		UARTflag = 1;
	}
	writeChar('\n');
}
