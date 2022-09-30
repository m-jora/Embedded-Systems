#ifndef _GPIO_H
#define _GPIO_H

#include "main.h"

#define set_outputs_macro(port, mask) ((port)|= (mask))
#define clear_outputs_macro(port, mask) ((port)&= (~(mask))

void GPIO_Output_Init(volatile GPIO_t *addr, uint8_t pin_mask);
void GPIO_Output_Set(volatile GPIO_t *addr, uint8_t pin_mask);
void GPIO_Output_Clear(volatile GPIO_t *addr, uint8_t pin_mask);

#endif
