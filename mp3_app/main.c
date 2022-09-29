#include <avr/io.h>
#include "main.h"
#include <util/delay.h>
#include "LEDS.h"
#include "uart.h"



int main(void)
{
	LEDS_Init(LED0_port, LED0_pin);
	LEDS_Init(LED1_port, LED1_pin);
	LEDS_Init(LED2_port, LED2_pin);
	LEDS_Init(LED3_port, LED3_pin);
	
	UART_Init(UART1, baud_rate);
	
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
		
		uint8_t value = UART_Receive(UART1);
		_delay_ms(100);
		UART_Transmit(UART1, value);
		_delay_ms(100);
    }
}

