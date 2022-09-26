#ifndef	_MAIN_H
#define _MAIN_H

#include <stdint.h>

#define F_CPU (16000000)
#define OSC_DIV (1)
//#define OSC_F(F_CPU / OSC_DIV)
//#define baud_rate ((OSC_F / (8 * (2 - U2X))
//#define UBRR_value((OSC_F / (8 * (2 - U2X) * baud_rate)) -1)


#define PIN_offset (0)
#define DDR_offset (1)
#define PORT_offset (2)

#define PC ((volatile GPIO_t *)(&PINC))
#define PB ((volatile GPIO_t *)(&PINB))
#define PE ((volatile GPIO_t *)(&PINE))
#define PA ((volatile GPIO_t *)(&PINA))


/*
1.	Done
2.	Done
3.	Done
4.	
5.	
6.	
7.	
8.	
9.	
10.	
11.	
12.	
*/



typedef struct GPIO{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;

#endif