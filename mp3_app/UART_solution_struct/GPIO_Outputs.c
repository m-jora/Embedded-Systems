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

uint8_t Read_Pin(volatile GPIO_t * port_addr, uint8_t pin_mask){
	uint8_t temp8;
	temp8 = (port_addr->GPIO_PIN);
	if((temp8&pin_mask)==0) temp8 = 0; // return ‘0’ if pin==0
	else temp8 = 1;                    // return ‘1’ if pin==1
	return temp8;
}