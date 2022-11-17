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
#include "Directory_Functions_struct.h"

int main(void)
{
	char string_in_SRAM [63] = "This is a string in SRAM - Garrett Mason & Harrison Heselbarth";
	// Local variables
	char * p_buffer;
	p_buffer = Export_print_buffer();
	uint8_t error_flag;
	uint8_t array[512];
	
	// Initializations
	UART_Init(UART1,9600);
	uint8_t SPI_error = SPI_Master_Init(SPI0_base, 400000UL);
	uint8_t SD_error = SD_Card_Init();
	uint8_t SPI_error2 = SPI_Master_Init(SPI0_base, 25000000UL);
	uint8_t TWI_error = TWI_Master_Init(TWI1_Base, 2500UL);
	SD_CS_active(SD_CS_port, SD_CS_pin);
	uint8_t Mount_error = mount_drive(array);
	sprintf(p_buffer, "\n\rMount Error: 0x%X\n\r", Mount_error);
	UART_Transmit_String(UART1, 0, p_buffer);
	uint32_t current_dir = First_Sector(0);
	sprintf(p_buffer, "\n\rcurrent_dir 0x%X\n\r", current_dir);
	UART_Transmit_String(UART1, 0, p_buffer);
	uint32_t entry_num, clus_num;

	
	/*uint8_t array[3];*/
	//STA013_Master_Init(p_buffer, array);
	
	while(1){
		// Get number of entries
		uint16_t num_of_entries = Print_Directory(current_dir, array);
		sprintf(p_buffer, "\n\rnum_of_entries:0x%X\n\r", num_of_entries);
		UART_Transmit_String(UART1, 0, p_buffer);
		
		// Prompt user for entry number
		do{
			sprintf(p_buffer, "\n\rEnter an entry number:\n\r");
			UART_Transmit_String(UART1, 0, p_buffer);
			entry_num = Long_Serial_Input(UART1);
			sprintf(p_buffer, "\n\rThe entry number you entered: 0x%X\n\r", entry_num);
			UART_Transmit_String(UART1, 0, p_buffer);
		}
		while(entry_num > num_of_entries);
		
		clus_num = Read_Dir_Entry(current_dir, entry_num, array); // Bit 28: 1 for dir 0 for file // Bit 31: 1 for err
		
		// If dir
		if((clus_num & 0x10000000) == 0x10000000){
			current_dir = First_Sector(clus_num);
		}
		// If file
		else{
			Print_File(clus_num, array);
		}
		
		
// 		// Prompt user for read block address
// 		sprintf(p_buffer, "\n\rEnter a block number to read:\n\r");
// 		UART_Transmit_String(UART1, 0, p_buffer);
// 		uint32_t user_input = Long_Serial_Input(UART1);
// 		sprintf(p_buffer, "\n\rThe block number you entered: 0x%X\n\r", user_input);
// 		UART_Transmit_String(UART1, 0, p_buffer);
//  		
// 		// Read block
// 		SD_CS_active(SD_CS_port, SD_CS_pin);
// 		error_flag = Send_Command(17, user_input);		
// 		error_flag = Read_Block(512, array);
// 		SD_CS_inactive(SD_CS_port, SD_CS_pin);
// 		
// 		// 	Printing
// 		print_memory(UART1, 512, array); 		
		
	}
}
