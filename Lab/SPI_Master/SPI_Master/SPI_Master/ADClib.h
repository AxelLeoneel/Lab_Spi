/*
 * ADClib.h
 *
 * Created: 18/07/2025 11:10:37
 *  Author: axell
 */ 


#ifndef ADCLIB_H_
#define ADCLIB_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void configADC(uint8_t canal);
void configChannel(uint8_t canal);

#endif /* ADCLIB_H_ */