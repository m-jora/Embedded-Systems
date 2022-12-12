/*
 * GPIO_Outputs.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "GPIO_Outputs.h"

void GPIO_Output_Init(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//Set Port Pin to be an output with initial value '1'
	(port_addr->GPIO_DDR)|=(pin_mask);    // Set DDRx to make port pin an output
}


void GPIO_Output_Set(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//control = 0, LED off; control = 1, LED on 
		(port_addr->GPIO_PORT)|=(pin_mask);    // Set PORTx PORTx pin to value of '1'
}

void GPIO_Output_Clear(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//control = 0, LED off; control = 1, LED on
		(port_addr->GPIO_PORT)&=~(pin_mask);    // Clear PORTx pin to value of '0'
}
