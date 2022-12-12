/*
 * Exp6_example.c
 *
 * Created: 11/30/2022 4:24:42 PM
 * Author : younger
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "LEDS.h"
#include "UART.h"
#include "UART_Print.h"
#include "SPI.h"
#include "OLED.h"
#include "Ex6Funcs.h"
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
	// Initialized LED1-3
	LEDS_Off(LED1_port,LED1_pin); //LSB
	LEDS_Init(LED1_port,LED1_pin);
	LEDS_Off(LED2_port,LED2_pin);
	LEDS_Init(LED2_port,LED2_pin);
	LEDS_Off(LED3_port,LED3_pin);
	LEDS_Init(LED3_port,LED3_pin); // MSB
	// Initialize Switches 1-3
	Input_Init(PB, SW1);
	Input_Init(PA, SW2);
	Input_Init(PA, SW3);
	Pull_Up_Enable(PA, SW3); //Switch 3 is active low for some reason
	
// 	Display_Binary_On_LEDs(0);
	
	
	
	//***** Main Loop: LED0 should always flash at around 0.5s on and 0.5s off  ****
	//***** Do not change any of the code in the while(1) super loop ****
    while (1) 
    {
		LEDS_On(LED0_port,LED0_pin);
		_delay_ms(500);
		LEDS_Off(LED0_port,LED0_pin);
		_delay_ms(500);
		
		// Testing switch 1
		uint8_t temp8=Read_Pin(PB,SW1);
		if(temp8==0)
		{
			// Switch is pressed
			LEDS_On(LED1_port,LED1_pin);
		}
		
		// Testing switch 2
		temp8=Read_Pin(PA,SW2);
		if(temp8==0)
		{
			// Switch is pressed
			LEDS_On(LED2_port,LED2_pin);
		}
		
		// Testing switch 3
		temp8=Read_Pin(PA,SW3);
		if(temp8==0)
		{
			// Switch is pressed
			LEDS_On(LED3_port,LED3_pin);
		}
		
    }
}

