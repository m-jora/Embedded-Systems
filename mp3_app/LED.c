#include "LEDS.h"
#include "gpio.h"

void LEDS_Init(volatile GPIO_t * port_addr, uint8_t LED_pin)
{
    GPIO_Output_Init(port_addr, LED_pin);
}

void LEDS_Off(volatile GPIO_t * port_addr, uint8_t LED_pin){
	GPIO_Output_Clear(port_addr, LED_pin);
}

void LEDS_On (volatile GPIO_t * port_addr, uint8_t LED_pin){
	GPIO_Output_Set(port_addr, LED_pin);
}