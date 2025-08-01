/*
 * SPI.h
 *
 * Created: 25/07/2025 11:04:26
 *  Author: axell
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

typedef enum{					
	
	SPI_osc_div2		= 0b01010000,
	SPI_osc_div4		= 0b01010001,
	SPI_osc_div8		= 0b01010010,
	SPI_osc_div16		= 0b01010011,
	SPI_osc_div32		= 0b01010100,
	SPI_osc_div64		= 0b01010101,
	SPI_osc_div128		= 0b01010110,
	SPI_slave_select	= 0b01000000
	
}SPI_Type;

typedef enum{				
	
	SPI_msb		= 0b00000000,
	SPI_lsb		= 0b00100000
	
}SPI_data_order;

typedef enum{				
	
	SPI_high	= 0b00001000,
	SPI_low		= 0b00000000
	
}SPI_polarityCLK;

typedef enum{					
	
	SPI_first	= 0b00000000,
	SPI_last	= 0b00000100
	
}SPI_phaseCLK;

void initSPI(SPI_Type, SPI_data_order, SPI_polarityCLK, SPI_phaseCLK);
void SPIwrite(uint8_t data);
uint8_t SPIread(void);

#endif /* SPI_H_ */