#ifndef _LEDS_H
#define _LEDS_H

#include "main.h"
#define LED0_port (PC)
#define LED0_pin (1<<7)

#define LED1_port (PB)
#define LED1_pin (1<<3)

#define LED2_port (PE)
#define LED2_pin (1<<4)

#define LED3_port (PA)
#define LED3_pin (1<<7)

LEDS_Init(volatile GPIO_t * port_addr, uint8_t LED_pin);

#endif