/*
 * Exp6_example.c
 *
 * Created: 11/30/2022 4:24:42 PM
 * Author : youngerr
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "LEDS.h"
#include "UART.h"
#include "UART_Print.h"
#include "SPI.h"
#include "OLED.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

const char Hello_String[15]PROGMEM={"Hello World!\n\r"};

int main(void)
{
    char *print_p;
	print_p=Export_print_buffer();
	//*** UART_Print and OLED functions added for debugging tools and messages ****
	UART_Init(UART1,9600);
	SPI_Master_Init(OLED_SPI_Port,10000000);
	OLED_Init(OLED_SPI_Port);
	Copy_String_to_Buffer(Hello_String,0,print_p);
	UART_Transmit_String(UART1,0,print_p);
	OLED_Set_Line_0(OLED_SPI_Port);
	OLED_Transmit_String(OLED_SPI_Port,12,print_p);
	//**** LED initialization ******
	LEDS_Off(LED0_port,LED0_pin);
	LEDS_Init(LED0_port,LED0_pin);
	//***** Main Loop: LED0 should always flash at around 0.5s on and 0.5s off  ****
	//***** Do not change any of the code in the while(1) super loop ****
    while (1) 
    {
		LEDS_On(LED0_port,LED0_pin);
		_delay_ms(500);
		LEDS_Off(LED0_port,LED0_pin);
		_delay_ms(500);
    }
}

