/*
 * LEDS.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: youngerr
 */ 

//#include <asf.h>
//#include "board_mp3.h"
#include "LEDS.h"
#include "GPIO_Outputs.h"

void LEDS_Init(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//Set Port Pin to be an output with initial value '1'
	//port_addr->GPIO_PORT|=(pin_mask);    // Set PORTx for initial value of '1'
	GPIO_Output_Init(port_addr, pin_mask);    // Set DDRx to make port pin an output
}


void LEDS_Off(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//control = 0, LED off; control = 1, LED on 
		GPIO_Output_Set(port_addr, pin_mask);    // Set PORTx PORTx pin to value of '1'
}

void LEDS_On(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//control = 0, LED off; control = 1, LED on
		GPIO_Output_Clear(port_addr, pin_mask);   // Clear PORTx pin to value of '0'
}
