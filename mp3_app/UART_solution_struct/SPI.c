#include "SPI.h"
#include "board_struct.h"
#include "GPIO_Outputs.h"

uint8_t SPI_Master_Init(uint8_t volatile *SPI_addr, uint32_t clock_rate)
{
	uint8_t return_value = 0x00;
	uint32_t divider = (F_CPU / F_DIV) / (clock_rate);
	
	if(divider < 2){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 1;
	}
	else if(divider < 4){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 0;
	}
	else if(divider < 8){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 1<<0);
		*(SPI_addr+SPSR) = 1;
	}
	else if(divider < 16){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 1<<0);
		*(SPI_addr+SPSR) = 0;
	}
	else if(divider < 32){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 2<<0);
		*(SPI_addr+SPSR) = 1;
	}
	else if(divider < 64){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 2<<0);//Roger Changed
		*(SPI_addr+SPSR) = 0;
	}
	else if(divider < 128){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 3<<0);
		*(SPI_addr+SPSR) = 0;
	}
	else{
		return_value = clock_rate_error;
	}

	//Initializing MOSI and SCK
	if(SPI_addr == SPI0_base){
		//Initializing MOSI
		GPIO_Output_Set(PB, (1<<5));
		GPIO_Output_Init(PB, (1<<5));
		
		if(CPOL_bit == 0){
			//Setting SCK to idle state
			GPIO_Output_Clear(PB, (1<<7));
			GPIO_Output_Init(PB, (1<<7));
		}
		else{
			GPIO_Output_Clear(PB, (0<<7));
			GPIO_Output_Init(PB, (0<<7));
		}
	}
	else{
		//Initializing MOSI
		GPIO_Output_Set(PE, (1<<3));
		GPIO_Output_Init(PE, (1<<3));
		
		if(CPOL_bit == 0){
			//Setting SCK to idle state
			GPIO_Output_Clear(PD, (1<<7));
			GPIO_Output_Init(PD, (1<<7));
		}
		else{
			GPIO_Output_Clear(PD, (0<<7));
			GPIO_Output_Init(PD, (0<<7));
		}
	}
	
	return return_value;
}

uint8_t SPI_Transfer(uint8_t volatile *SPI_addr, uint8_t send_value){
	uint8_t status;
	
	*(SPI_addr + SPDR) = send_value;
	
	do{
		status = *(SPI_addr + SPSR);
	}while((status & 0x80) == 0);
	
	return *(SPI_addr + SPDR);
}

uint8_t Send_Command (uint8_t command, uint32_t argument){
	uint8_t return_value = 0x00;
	uint8_t illegal_command = 0xFF;
	uint8_t send_value;
	
	if(command < 64){
		return_value = no_errors;
	}
	else{
		return_value = illegal_command;
		return return_value;
	}
	
	send_value = 0x40 | command;
	SPI_Transfer(SD_SPI_port, send_value);
	for(uint8_t index = 0; index < 4; index++){
		send_value = (uint8_t)(argument >> (24 - (index * 8)));
		SPI_Transfer(SD_SPI_port, send_value);
	}
	
	if(command == 0){
		send_value = 0x95;
	}
	else if (command == 8){
		send_value = 0x87;
	}
	else{
		send_value = 0x01;
	}
	
	SPI_Transfer(SD_SPI_port, send_value);	
	return return_value;
}

uint8_t Receive_Response (uint8_t number_of_bytes, uint8_t * array_name) {
	uint8_t return_value = no_errors;
	uint8_t SPI_timeout_error = 0xFA;
	uint8_t SD_comm_error = 0xFE;
	uint8_t timeout = 0;
	uint8_t rcvd_value;
	
	do{
		rcvd_value = SPI_Transfer(SD_SPI_port, 0xFF);
		timeout++;
	}while((rcvd_value == 0xFF) && (timeout != 0));
	
	if(timeout == 0){
		return_value = SPI_timeout_error;
	}
	else if ((rcvd_value & 0xFE) != 0x00){
		*array_name = rcvd_value;
		return_value = SD_comm_error;
	}
	else{
		*array_name = rcvd_value;
		if(number_of_bytes > 1){
			for(uint16_t index = 1; index < number_of_bytes; index++){
				rcvd_value = SPI_Transfer(SD_SPI_port, 0xFF);
				*(array_name + index) = rcvd_value;
			}
		}
	}
	rcvd_value = SPI_Transfer(SD_SPI_port, 0xFF);
	return return_value;
}
