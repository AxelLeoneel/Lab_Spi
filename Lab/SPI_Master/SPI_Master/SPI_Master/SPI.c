/*
 * SPI.c
 *
 * Created: 25/07/2025 11:04:17
 *  Author: axell
 */ 

#include "SPI.h"

void initSPI(SPI_Type Type, SPI_data_order Order, SPI_polarityCLK Polarity, SPI_phaseCLK Phase){
	
	//Pines a utilizar
	//DDB2 = SS
	//DDB3 = MOSI
	//DDB4 = MISO
	//DDB5 = SCK
	
	if(Type & (1 << MSTR)){										//Si el tipo = maestro
		DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);		//Config para maestro
		DDRB &= ~(1 << DDB4);									//MISO
		SPCR |= (1 << MSTR);									//Maestro
		
		uint8_t select = Type & 0b00000111;
		switch(select){											//Selector de frecuencia para la vel
			case 0:
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR |= (1 << SPI2X);
			break;
			case 1:
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR &= ~(1 << SPI2X);
			break;
			case 2:
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;
			case 3:
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			case 4:
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;
			case 5:
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			case 6:
			SPCR |= (1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
		}
	}
	else{															//Si es slave
		DDRB |= (1 << DDB4);										//MISO
		DDRB &= ~((1 << DDB3) | (1 << DDB5) | (1 << DDB2));			//Config para slave
		SPCR &= ~(1 << MSTR);										//Slave
	}
	
	SPCR |= (1 << SPE) | Order | Polarity | Phase;					//Habilitamos SPI
}

void SPIwrite(uint8_t data){
	
	SPDR = data;													//Cargamos la informacion
}																	//a registro de datos

uint8_t SPIread(void){
	
	while(!(SPSR & (1 << SPIF)));									//Esperamos hasta haber
	return(SPDR);													//recibido el dato y luego
}																	//leemos la informacion obtenida