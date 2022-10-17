#include "SPI.h"
#include "board_struct.h"

#define SD_CS_port (PB) //Was PIN_B
#define SD_CS_pin (1<<4)
#define SD_CS_SCK (1<<7)
#define no_errors (0x00)
#define incompatible_voltage () // DECLARE ME!!

void SD_CS_active (volatile uint8_t *port, uint8_t pin){
	GPIO_Output_Clear(port, pin);
}

void SD_CS_inactive (volatile uint8_t *port, uint8_t pin){
	GPIO_Output_Set(port, pin);
}

uint8_t SD_Card_Init (void){
	uint8_t error_status = no_errors;
	uint8_t error_flag = no_errors;
	uint8_t rec_array[100];
	uint8_t ACMD41_arg;
	
	if(error_status == no_errors){ //
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
	
		SPI_Master_Init(SPI0_base, 400000UL);
		// Send at least 74 SCK pulses
		for(uint8_t i = 0; i < 10; i++){
			Send_Command(0x00, 0xFF);//May not work?---------
		}
	}
	
	if(error_status == no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(CMD0, 0x00000000);		
		if(error_flag != 0x01){
			error_flag = Receive_Response(5, rec_array);
		}
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
	}
	
	if(error_status == no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(CMD8, 0x000001AA);
		if(error_flag == no_errors){ //Check if R1 is not 0x01 (error)
			error_flag = Receive_Response(5, rec_array);
		}
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
		if((error_flag == no_errors) && (rec_array[0] == 0x01)){
			if((rec_array[3] == 0x01) && (rec_array[4] == 0xAA)){
				ACMD41_arg = 0x40000000; // High-Capacity Support
			}
			else{
				error_status = incompatible_voltage;//Need to declare this
			}
		}
		else if(rec_array[0] == 0x05){
			error_status = no_errors;
			ACMD41_arg = 0x00000000;
			SD_Card_Type_g = Standard_Capacity; //Not defined
		}
		else{
			error_status = error_flag;
		}
	}
}