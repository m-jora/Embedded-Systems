#include "gpio.h"

void GPIO_Output_Init(volatile GPIO_t* base_addr,uint8_t pin_mask){
	(base_addr -> GPIO_DDR) |= pin_mask;
}

void GPIO_Output_Set(volatile GPIO_t *addr, uint8_t pin_mask){
	(addr -> GPIO_DDR) |= pin_mask;
}

void GPIO_Output_Clear(volatile GPIO_t *addr, uint8_t pin_mask){
	(addr -> GPIO_DDR) &= ~(pin_mask);
}
