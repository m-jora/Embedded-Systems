#include <avr/io.h>
#include "board_struct.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"
#include "UART.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "UART_Print.h"
#include "print_memory.h"
#include "Long_Serial_In.h"
#include <stdio.h>
#include "SD.h"



const char test_string[15] PROGMEM = "Hello World!\n\r";


int main(void)
{
	/* Replace with your application code */
	char temp8;
	char string_in_SRAM [41] = "This is a string in SRAM - Garrett Mason & Harrison Heselbarth";
	char *string_p;
	uint32_t temp32;
	LEDS_Off(LED0_port, LED0_pin);
	LEDS_Init(LED0_port, LED0_pin);
	
	UART_Init(UART1,9600);
	uint8_t SPI_error = SPI_Master_Init(SPI0_base, 400000UL);
	uint8_t SD_error = SD_Card_Init();
	uint8_t SPI_error2 = SPI_Master_Init(SPI0_base, 25000000UL);
	
	char * p_buffer;
	p_buffer = Export_print_buffer();
	sprintf(p_buffer, "\n\rSPI_error: 0x%X\n\r", SPI_error);
	UART_Transmit_String(UART1, 0, p_buffer);
	sprintf(p_buffer, "SD_error: 0x%X\n\r", SD_error);
	UART_Transmit_String(UART1, 0, p_buffer);
	sprintf(p_buffer, "SPI_error2: 0x%X\n\r", SPI_error2);
	UART_Transmit_String(UART1, 0, p_buffer);

	LEDS_On(LED0_port, LED0_pin);
	temp8=UART_Receive(UART1);
	UART_Transmit(UART1,temp8);
		
		
	
	
	while (0)
	{
		temp8=UART_Receive(UART1);
		UART_Transmit(UART1,temp8);
		
		LEDS_On(LED0_port, LED0_pin);
		UART_Transmit(UART1,'U');
		_delay_ms(100);
		
		LEDS_Off(LED0_port, LED0_pin);
		_delay_ms(900);
		
		LEDS_On(LED1_port, LED1_pin);
		UART_Transmit(UART1,CR);
		_delay_ms(100);
		
		LEDS_Off(LED1_port, LED1_pin);
		_delay_ms(900);
		
		LEDS_On(LED2_port, LED2_pin);
		UART_Transmit(UART1,LF);
		_delay_ms(100);
		
		LEDS_Off(LED2_port, LED2_pin);
		_delay_ms(900);
		
		LEDS_On(LED3_port, LED3_pin);
		UART_Transmit(UART1,':');
		_delay_ms(100);
		
		LEDS_Off(LED3_port, LED3_pin);
		_delay_ms(900);
		temp32=Long_Serial_Input(UART1);
		sprintf(string_p,"%lu \n\r",temp32);   // %lu is used for an unsigned long
		UART_Transmit_String(UART1,0,string_p);
		
	}
}


