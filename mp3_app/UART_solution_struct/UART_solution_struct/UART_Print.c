/*
 * Print_Buffer.c
 *
 * Created: 9/4/2020 4:34:38 PM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "UART.h"
#include <avr/pgmspace.h>

static char print_buffer[80];

char * Export_print_buffer(void)
{
	
	return print_buffer;
}

uint8_t Copy_String_to_Buffer(const char flash_string[], uint8_t num_bytes, char buffer_p[])
{
	uint8_t index;
	char value;
	if(num_bytes==0)
	{
		index=0;
		do
		{
			value=pgm_read_byte(&flash_string[index]);
			buffer_p[index]=value;
			index++;
		} while (value!=0);
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			value=pgm_read_byte(&flash_string[index]);
			buffer_p[index]=value;
		}
		index=num_bytes;
	}
	return index;
}

void UART_Transmit_String(volatile UART_t * UART_addr, uint8_t num_bytes, char * string_name)
{
	char temp8;
	int8_t index;
	if(num_bytes==0)
	{
		temp8=*string_name;
		while (temp8!=0)
		{
			UART_Transmit(UART_addr, temp8);
			string_name++;
			temp8=*string_name;
		}
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			UART_Transmit(UART_addr, string_name[index]);
		}
	}
}

