/*
 * SPI_lib.c
 *
 * Created: 25/07/2025 15:43:21
 *  Author: axell
 */ 

#include "SPI_lib.h"
// Function to initialize SPI configuration on uC
void initSPI(Spi_Type type, Spi_Data_Order dataorder, Spi_Clock_Polarity polarity, Spi_Clock_Phase phase)
{
	// PB2: Slave Select
	// PB3: MOSI
	// PB4: MISO
	// PB5: SCK
	
	// Master mode
	if (type & (1 << MSTR))
	{
		// OUTPUT
		DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2);
		// INPUT
		DDRB &= ~ (1 << DDB4);
		// Master mode
		SPCR |= (1 << MSTR);
		// Select PreScaler
		uint8_t temporal = type & 0b00000111;
		// Cases
		switch(temporal)
		{
			case 0: // PreScaler = 2
				SPCR &= ~( (1 << SPR1) | (1 << SPR0) );
				SPSR |= (1 << SPI2X);
				break;
			case 1: // PreScaler = 4
				SPCR &= ~( (1 << SPR1) | (1 << SPR0) );
				SPSR &= ~(1 << SPI2X);
				break;
			case 2: // PreScaler = 8
				SPCR |= (1 << SPR0);
				SPCR &= ~(1 << SPR1);
				SPSR |= (1 << SPI2X);
				break;
			case 3: // PreScaler = 16
				SPCR |= (1 << SPR0);
				SPCR &= ~(1 << SPR1);
				SPSR &= ~(1 << SPI2X);
				break;
			case 4: // PreScaler = 32
				SPCR &= ~(1 << SPR0);
				SPCR |= (1 << SPR1);
				SPSR |= (1 << SPI2X);
				break;
			case 5: // PreScaler = 64
				SPCR &= ~(1 << SPR0);
				SPCR |= (1 << SPR1);
				SPSR &= ~(1 << SPI2X);
				break;
			case 6: // PreScaler = 128
				SPCR |= (1 << SPR0) | (1 << SPR1);
				SPSR &= ~(1 << SPI2X);
				break;
		}
	}
	else
	{
		// OUTPUT
		DDRB &= ~((1 << DDB5) | (1 << DDB3) | (1 << DDB2));
		// INPUT
		DDRB |= (1 << DDB4);
		// Slave mode
		SPCR &= ~ (1 << MSTR);
	}
	// Enable SPI, Data Order, Clock Polarity, Clock Phase
	SPCR |= (1 << SPE) | dataorder | polarity | phase;
	
}
// Function to send a byte of data
void spiWrite(uint8_t data)
{
	SPDR = data;
}
// Show if data is ready to be read
unsigned spiDataReady()
{
	if(SPSR & (1 << SPIF))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
// Function to read a byte of data
uint8_t spiRead(void)
{
	while( !(SPSR & (1 << SPIF)) ); // Wait for the receive to end
	return(SPDR); // Read the received data
}