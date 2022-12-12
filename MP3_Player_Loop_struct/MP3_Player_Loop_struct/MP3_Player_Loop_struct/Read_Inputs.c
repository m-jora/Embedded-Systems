/*
 * Read_Inputs.c
 *
 * Created: 11/17/2020 11:12:47 AM
 *  Author: youngerr
 */ 

#include "board_struct.h"


void GPIO_Input_Init(volatile GPIO_t *port_addr, uint8_t pin_mask)
{
    (port_addr->GPIO_DDR)&=(~pin_mask);
}


void GPIO_Pull_Up_Enable(volatile GPIO_t *port_addr, uint8_t pin_mask)
{
	(port_addr->GPIO_PORT)|=(pin_mask);    // Set PORTx pin to value of '1'
}

void GPIO_Pull_Up_Disable(volatile GPIO_t *port_addr, uint8_t pin_mask)
{
	(port_addr->GPIO_PORT)&=(~(pin_mask));    // Set PORTx pin to value of '1'
}

uint8_t GPIO_Read_Inputs(volatile GPIO_t *port_addr)
{
    return (port_addr->GPIO_PIN);  // returns the value in the PIN reg.
}

uint8_t GPIO_Read_Pin(volatile GPIO_t *port_addr, uint8_t pin_mask)
{
	uint8_t temp8;   
	temp8 = (port_addr->GPIO_PIN);     
	if((temp8&pin_mask)==0) temp8 = 0; // return ‘0’ if pin==0   
	else temp8 = 1;                    // return ‘1’ if pin==1
    return temp8;
}

