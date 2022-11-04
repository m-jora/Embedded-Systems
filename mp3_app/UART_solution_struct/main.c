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
#include "SPI.h"
#include "TWI.h"
#include "STA013.h"

int main(void)
{
	char string_in_SRAM [41] = "This is a string in SRAM - Garrett Mason & Harrison Heselbarth";
	// Local variables
	char * p_buffer;
	p_buffer = Export_print_buffer();
	uint8_t error_flag;
	uint8_t array[3];
	
	// Initializations
	UART_Init(UART1,9600);
	uint8_t SPI_error = SPI_Master_Init(SPI0_base, 400000UL);
	uint8_t SD_error = SD_Card_Init();
	uint8_t SPI_error2 = SPI_Master_Init(SPI0_base, 25000000UL);
	uint8_t TWI_error = TWI_Master_Init(TWI1_Base, 2500UL);
	
	// Testing
	/*sprintf(p_buffer, "\n\rSPI_error: 0x%X\n\r", SPI_error);
	UART_Transmit_String(UART1, 0, p_buffer);
	sprintf(p_buffer, "\n\rSD_error: 0x%X\n\r", SD_error);
	UART_Transmit_String(UART1, 0, p_buffer);
	sprintf(p_buffer, "\n\rSPI_error2: 0x%X\n\r", SPI_error2);
	UART_Transmit_String(UART1, 0, p_buffer);*/
	sprintf(p_buffer, "\n\rTWI_error: 0x%X\n\r", TWI_error);
	UART_Transmit_String(UART1, 0, p_buffer);
	
	STA013_Master_Init(p_buffer, array);
	
	while(0){
		// Prompt user for read block address
		sprintf(p_buffer, "\n\rEnter a block number to read:\n\r");
		UART_Transmit_String(UART1, 0, p_buffer);
		uint32_t user_input = Long_Serial_Input(UART1);
		sprintf(p_buffer, "\n\rThe block number you entered: 0x%X\n\r", user_input);
		UART_Transmit_String(UART1, 0, p_buffer);
		
		// Read block
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(17, user_input);		
		error_flag = Read_Block(512, array);
		
		// Printing
		print_memory(UART1, 512, array);		
	}
}
