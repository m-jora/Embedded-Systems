#ifndef	_MAIN_H
#define _MAIN_H

#include <stdint.h>

#define F_CPU (16000000)
#define OSC_DIV (1)

#define PIN_offset(0)
#define DDR_offset(1)
#define PORT_offset(2)

#define PC ((volatile GPIO_t *)(&PINC))
#define PB ((volatile GPIO_t *)(&PINB))
#define PE ((volatile GPIO_t *)(&PINE))
#define PA ((volatile GPIO_t *)(&PINA))





typedef struct GPIO{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;

#endif