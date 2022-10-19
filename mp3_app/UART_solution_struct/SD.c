#include "SPI.h"
#include "board_struct.h"
#include "SD.h"

#define SD_CS_SCK (1<<7)
#define no_errors (0x00)
#define incompatible_voltage (0xFD) // DECLARE ME!!
#define High_Capacity (0)
#define Standard_Capacity (9)

static uint8_t SD_Card_Type_g;

void SD_CS_active (volatile uint8_t *port, uint8_t pin){
	GPIO_Output_Clear(port, pin);
}

void SD_CS_inactive (volatile uint8_t *port, uint8_t pin){
	GPIO_Output_Set(port, pin);
}

uint8_t SD_Card_Init (void){
	uint8_t error_status = no_errors;
	uint8_t error_flag = no_errors;
	uint8_t rec_array[5];
	uint8_t ACMD41_arg;
	uint8_t timeout = 0;
	
	GPIO_Output_Init(SD_CS_port, SD_CS_pin);
	
	if(error_status == no_errors){ //
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
	
		SPI_Master_Init(SD_SPI_port, 400000UL);
		// Send at least 74 SCK pulses
		for(uint8_t i = 0; i < 10; i++){
			SPI_Transfer(SPI0_base, 0xFF);
		}
	}
	
	// CMD0
	if(error_status == no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(0, 0x00000000);
		error_status = error_flag;	
		if(error_flag != 0x01){
			error_flag = Receive_Response(1, rec_array); //5 after c8 C58
		}
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
		error_status = error_flag;
	}
	
	// CMD8
	if(error_status == no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(8, 0x000001AA);
		if(error_flag == no_errors){ //Check if R1 is not 0x01 (error)
			error_flag = Receive_Response(5, rec_array);
		}
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
		if((error_flag == no_errors) && (rec_array[0] == 0x01)){ //Is 0x00 when we want it to be 0x01
			if((rec_array[3] == 0x01) && (rec_array[4] == 0xAA)){ //Not entering if when should be!
				ACMD41_arg = 0x40000000; // High-Capacity Support
				SD_Card_Type_g = High_Capacity;
			}
			else{
				error_status = incompatible_voltage;
			}
		}
		else if(rec_array[0] == 0x05){
			error_status = no_errors;
			ACMD41_arg = 0x00000000;
			SD_Card_Type_g = Standard_Capacity;
		}
		else{
			error_status = error_flag;
		}
	}
	
	// CMD58
	if(error_status == no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(58, 0x00000000);
		if(error_flag == no_errors){ //Check if R1 is not 0x01 (error)
			error_flag = Receive_Response(5, rec_array);
		}
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
		if((error_flag == no_errors) && (rec_array[0] == 0x01)){
			if((rec_array[3] & 0b00110000 != 0b00110000)){
				error_status = incompatible_voltage;
			}
		}
	}
	
	// ACMD41 -------------Issue should be 69 40 not 69 00 on second round, first row
	if(error_status == no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		
		do{
			error_flag = Send_Command(55, 0x00000000);
			if(error_flag == no_errors){ //Check if R1 is not 0x01 (error)
				error_flag = Receive_Response(1, rec_array);
			}
		
			if((error_flag == no_errors) && (rec_array[0] == 0x01)){
				error_status = no_errors;
				error_flag = Send_Command(41, 0x40000000); // 2nd arg was ACMD41_arg
				error_flag = Receive_Response(1, rec_array);
				if(rec_array[0] != 0x00 && rec_array[0] != 0x01){
					error_status = 0xFE;
					break;
				}
				
				timeout++;
			}
		}while((rec_array[0] != 0x00) && (timeout != 0));	
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
	}
	
	// CMD58 (didn't quite match up, 00 and C0 instead of 01 and 00 at end of second row)
	if(error_status == no_errors){//&& ACMD41_arg == 0x40000000){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		error_flag = Send_Command(58, 0x00000000);
		if(error_flag == no_errors){ //Check if R1 is not 0x01 (error)
			error_flag = Receive_Response(5, rec_array);
		}
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
		if((error_flag == no_errors) && (rec_array[0] == 0x01)){
			if((rec_array[4] & 0b11000000 != 0b11000000)){
				SD_Card_Type_g = High_Capacity;
			}
			else if(rec_array[4] & 0b11000000 != 0b10000000){
				SD_Card_Type_g = Standard_Capacity;
			}
			else{
				error_status = 0xFF;
			}
		}
	}
	return error_status;
}

uint8_t Read_Block (uint16_t number_of_bytes, uint8_t * array){
	uint8_t rcvd_value;
	uint8_t timeout = 0;
	uint8_t error_flag;
	do{
		rcvd_value = SPI_Transfer(SPI0_base, 0xFF);
		timeout++;
	}while(rcvd_value == 0xFF && timeout != 0);
	
	if(rcvd_value == 0x00){
		do{
			rcvd_value = SPI_Transfer(SPI0_base, 0xFF);
		}while(rcvd_value == 0xFF);
		
		if(rcvd_value == 0xFE){
			for(uint16_t index = 0; index < number_of_bytes; index++){
				rcvd_value = SPI_Transfer(SPI0_base, 0xFF);
				array[index] = rcvd_value;
			}
			
			// Read and discard
			rcvd_value = SPI_Transfer(SPI0_base, 0xFF);
			rcvd_value = SPI_Transfer(SPI0_base, 0xFF);
			rcvd_value = SPI_Transfer(SPI0_base, 0xFF);
			
			
		}
		return rcvd_value;
	}
}