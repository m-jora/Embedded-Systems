#include "STA013.h"
#include "TWI.h"
#include <stdio.h>



void STA013_Master_Init(char * prnt_bffr, uint8_t * array_name){	
	uint8_t i = 255; /*This value is the number of attempts*/
	uint8_t error = 0xFF;
	uint8_t no_errors = 0;
	
	// Pulsing low
	GPIO_Output_Init(PB, (1<<1));
	for(uint8_t i = 0; i < 255; i++){
		GPIO_Output_Clear(PB, (1<<1));
	}
	GPIO_Output_Set(PB, (1<<1));
	
	do
	{
		error=TWI_Master_Transmit(TWI1_Base,0x43,0,array_name);
		i--;
	}while((error!=0)&&(i!=0));
	
	i=255;
	do
	{
		error = TWI_Master_Receive(TWI1_Base,0x43,0,0,1,array_name);
		sprintf(prnt_bffr, "Error Value = 0x%X\n\r", error);
		UART_Transmit_String(UART1,0,prnt_bffr);
		i--;
	}while((error!=no_errors)&&(i!=0));
	sprintf(prnt_bffr, "Received Value = %X\n\r", array_name[0]);
	UART_Transmit_String(UART1,0,prnt_bffr);
}