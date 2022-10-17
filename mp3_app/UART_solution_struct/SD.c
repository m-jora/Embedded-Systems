#include "SPI.h"
#include "board_struct.h"

#define SD_CS_port (PB) //Was PIN_B
#define SD_CS_pin (1<<4)
#define no_errors (0x00)

void SD_CS_active (volatile uint8_t *port, uint8_t pin){
	GPIO_Output_Clear(port, pin);
}

void SD_CS_inactive (volatile uint8_t *port, uint8_t pin){
	GPIO_Output_Set(port, pin);
}

uint8_t SD_Card_Init (void){
	uint8_t error_status = no_errors;
	uint8_t response;
	
	if(error_status != no_errors){ //
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
	
		// Send at least 74 SCK pulses
		for(uint8_t i = 0; i < 80; i++){
			// HOW TO???---------------------------
		}
	}
	
	if(error_status != no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		response = Send_Command(CMD0, 0x00000000);
		if(response != 0x01){
			error_status = 0xFF;
		}
		
		SD_CS_inactive(SD_CS_port, SD_CS_pin);
	}
	
	if(error_status != no_errors){
		SD_CS_active(SD_CS_port, SD_CS_pin);
		response = Send_Command(CMD8, 0x48000001AA87);
		if((response & ~0x00FFFFFFFFFFF) != 0x0100000000000){ //NOT DONE
			error_status = 0xFF;
		}
		
	}
}