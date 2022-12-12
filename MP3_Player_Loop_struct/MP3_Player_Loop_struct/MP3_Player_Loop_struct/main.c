/*
 * UART_solution_struct.c
 *
 * Created: 10/5/2021 3:06:09 PM
 * Author : ryoun
 */ 

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
#include "SPI.h"
#include "SDCard.h"
#include "OLED.h"
#include "Directory_Functions_struct.h"
#include "File_System_struct.h"
#include "TWI.h"
#include "STA013_Config.h"
#include "Play_Song.h"


const char test_string[28] PROGMEM = {"SD Initialization Program\n\r\0"};
const char LSI_Prompt[16] PROGMEM = {"Enter block #: "};
const char Complete[9] PROGMEM = {"  OK!\n\r\0"};
const char High_Cap[15] PROGMEM = {"High Capacity\0"};
const char Stnd_Cap[19] PROGMEM = {"Standard Capacity\0"};

uint8_t buffer1_g[512];
uint8_t buffer2_g[512];


int main(void)
{
	char string_in_SRAM [63] = "This is a string in SRAM - Garrett Mason & Harrison Heselbarth";
	char temp8,error_flag;
	uint32_t input32, Current_directory, num_entries, Entry_clus;
	char *string_p;
	FS_values_t * Drive_p;
	LEDS_Off(LED0_port, LED0_pin);
	LEDS_Init(LED0_port, LED0_pin);
	LEDS_Off(LED1_port, LED1_pin);
	LEDS_Init(LED1_port, LED1_pin);
	LEDS_Off(LED2_port, LED2_pin);
	LEDS_Init(LED2_port, LED2_pin);
	LEDS_Off(LED3_port, LED3_pin);
	LEDS_Init(LED3_port, LED3_pin);
	UART_Init(UART1,9600);
	string_p=Export_print_buffer();
	Copy_String_to_Buffer(test_string,0,string_p);
	UART_Transmit_String(UART1,0,string_p);
	
	LEDS_On(LED0_port, LED0_pin);
	_delay_ms(100);
	LEDS_Off(LED0_port, LED0_pin);
	error_flag=SPI_Master_Init(SPI0,400000);
	if(error_flag!=no_errors)
	{
		while(1);
		LEDS_On(LED1_port, LED1_pin);
	}
	OLED_Init(OLED_SPI_Port);
	OLED_Set_Line_0 (OLED_SPI_Port);
	Copy_String_to_Buffer(test_string,0,string_p);
	OLED_Transmit_String(OLED_SPI_Port,7,string_p);
	error_flag=SD_Card_Init();
	if(error_flag!=no_errors)
	{
		while(1);
		LEDS_On(LED2_port, LED2_pin);
	}
	error_flag=SPI_Master_Init(SPI0,10000000);
	Copy_String_to_Buffer(Complete,0,string_p);
	OLED_Transmit_String(OLED_SPI_Port,5,string_p);
	OLED_Set_Line_1 (OLED_SPI_Port);
	temp8=Return_SD_Card_Type();
	if(temp8==Standard_Capacity)
	{
		Copy_String_to_Buffer(Stnd_Cap,0,string_p);
		OLED_Transmit_String(OLED_SPI_Port,0,string_p);
	}
	else
	{
		Copy_String_to_Buffer(High_Cap,0,string_p);
		OLED_Transmit_String(OLED_SPI_Port,0,string_p);
	}
	LEDS_Off(LED0_port, LED0_pin);
	
	TWI_Master_Init(STA013_TWI_Port,50000);
	error_flag=STA013_Init();
	if(error_flag!=no_errors)
	{
		while(1);
		LEDS_On(LED2_port, LED2_pin);
	}
	
	Mount_Drive(buffer1_g);
	Drive_p=Export_Drive_values();
	Current_directory=Drive_p->FirstRootDirSec;
	while (1)
	{
			sprintf(string_p,"Current Directory = %lu\n\r",Current_directory);
			UART_Transmit_String(UART1,0,string_p);
			num_entries=Print_Directory(Current_directory, buffer1_g);
			sprintf(string_p,"Enter Selection = ");
			UART_Transmit_String(UART1,0,string_p);
			input32=(uint16_t)Long_Serial_Input(UART1);
			if(input32<=num_entries)
			{
				Entry_clus=Read_Dir_Entry(Current_directory, input32, buffer1_g);
				if((Entry_clus&directory_bit)==directory_bit)
				{
					Entry_clus&=0x0FFFFFFF;
					Current_directory=First_Sector(Entry_clus);
				}
				else
				{
					/*Open_File(Entry_clus, buffer1_g);*/
					Play_Song(Entry_clus);
				}
				
			}
			else
			{
				sprintf(string_p,"Invalid Selection\n\r");
				UART_Transmit_String(UART1,0,string_p);
			}
			
	}
}
