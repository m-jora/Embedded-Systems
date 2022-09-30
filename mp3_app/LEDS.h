#ifndef _LEDS_H
#define _LEDS_H

#include "main.h"
#include "port.h"

void LEDS_Init(volatile GPIO_t * port_addr, uint8_t LED_pin);

#endif
