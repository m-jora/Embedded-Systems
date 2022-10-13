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
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 1;
	}
	else if(divider < 16){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 0;
	}
	else if(divider < 32){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 1;
	}
	else if(divider < 64){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 0; //Might be 1?
	}
	else if(divider < 128){
		*(SPI_addr + SPCR) = ((1<<SPE) | (1<<MSTR) | (CPOL_bit << CPOL) | (CPHA_bit<<CPHA) | 0<<0);
		*(SPI_addr+SPSR) = 0;
	}
	else{
		return_value = clock_rate_error;
	}

	// Initializing MOSI and SCK
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