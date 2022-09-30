#include <avr/io.h>
#include "main.h"
#include <util/delay.h>
#include "LEDS.h"
#include "uart.h"
#include <stdio.h>
#include "UART_Print.h"

const char characters[30] = "Testing static character array";

int main(void)
{
	LEDS_Init(LED0_port, LED0_pin);
	LEDS_Init(LED1_port, LED1_pin);
	LEDS_Init(LED2_port, LED2_pin);
	LEDS_Init(LED3_port, LED3_pin);
	
	UART_Init(UART1, baud_rate);
	
	char * p_buffer;
	p_buffer = Export_print_buffer();
	//sprintf(p_buffer, "ello governor");
	
	uint8_t index = Copy_String_to_Buffer(characters, 0, p_buffer);
	
	
    /* Replace with your application code */
    while (1) 
    {
		/*
		LEDS_On(LED0_port, LED0_pin);
		LEDS_On(LED1_port, LED1_pin);
		LEDS_On(LED2_port, LED2_pin);
		LEDS_On(LED3_port, LED3_pin);
		_delay_ms(1000);
		LEDS_Off(LED0_port, LED0_pin);
		LEDS_Off(LED1_port, LED1_pin);
		LEDS_Off(LED2_port, LED2_pin);
		LEDS_Off(LED3_port, LED3_pin);
		_delay_ms(1000);	
		*/
		
		//uint8_t value = UART_Receive(UART1);
		//UART_Transmit(UART1, value);
		
		UART_Transmit_String(UART1, 0, p_buffer);
    }
}

