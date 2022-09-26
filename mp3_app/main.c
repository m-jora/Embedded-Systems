#include <avr/io.h>
#include "main.h"
#include <util/delay.h>
#include "LEDS.h"



int main(void)
{
	LEDS_Init(LED0_port, LED0_pin);
	LEDS_Init(LED1_port, LED1_pin);
	LEDS_Init(LED2_port, LED2_pin);
	LEDS_Init(LED3_port, LED3_pin);
	
    /* Replace with your application code */
    while (1) 
    {
		LEDS_On(LED0_port, LED0_pin);
		_delay_ms(1000);
		LEDS_Off(LED0_port, LED0_pin);
		_delay_ms(1000);	
    }
}

