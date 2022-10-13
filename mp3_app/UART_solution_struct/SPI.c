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
	uint8_t return_value;
	uint8_t no_errors = 0x00;
	uint8_t illegal_command = 0xFF; //Guessed on value for error
	uint8_t send_value; //Right type?
	
	if(command < 64){
		return_value = no_errors;
	}
	else{
		return_value = illegal_command;
		return return_value;
	}
	
	send_value = 0x40 | command;
	for(uint8_t index = 0; index < 4; index++){
		send_value = (uint8_t)(argument >> (24 - (index * 8)));
		SPI_Transfer(SD_SPI_port, send_value);
	}
	
	if(command == CMD0){
		send_value = 0x95;
	}
	else if (command == CMD8){
		send_value = 0x87;
	}
	else{
		send_value = 0x01;
	}
	
	SPI_Transfer(SD_SPI_port, send_value);	
	return return_value;
}


