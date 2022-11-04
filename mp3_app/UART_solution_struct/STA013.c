#include "STA013.h"
#include "TWI.h"
#include <stdio.h>
#include <avr/pgmspace.h>

extern const uint8_t CONFIG[];
extern const uint8_t CONFIG2[];


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
	
// 	array_name[0]=1;
// 	do
// 	{
// 		error=TWI_Master_Transmit(TWI1_Base,0x43,1,array_name);
// 		sprintf(prnt_bffr, "Transmit Error Value = 0x%X\n\r", error);
// 		UART_Transmit_String(UART1,0,prnt_bffr);
// 		i--;
// 	}while((error!=0)&&(i!=0));
// 	
// 	i=255;
// 	do
// 	{
// 		error = TWI_Master_Receive(TWI1_Base,0x43,0,0,1,array_name);
// 		sprintf(prnt_bffr, "Recieve Error Value = 0x%X\n\r", error);
// 		UART_Transmit_String(UART1,0,prnt_bffr);
// 		i--;
// 	}while((error!=no_errors)&&(i!=0));
// 	sprintf(prnt_bffr, "Received Value = %X\n\r", array_name[0]);
// 	UART_Transmit_String(UART1,0,prnt_bffr);

	uint8_t timeout;
	uint8_t status;
	uint16_t index=0;
	
	// CONFIG init
	do
	{
		array_name[0]=pgm_read_byte(&CONFIG[index]);  // internal reg. addr
		index++;
		array_name[1]=pgm_read_byte(&CONFIG[index]);  // value for the reg.
		index++;
		timeout=50;
		do
		{
			status=TWI_Master_Transmit(TWI1_Base,0x43,2,array_name);
			timeout--;
		}while((status!=no_errors)&&(timeout!=0));
	} while((array_name[0]!=0xFF)&&(timeout!=0));
	if(timeout!=0){
		sprintf(prnt_bffr,"Config sent...\n\r");
		UART_Transmit_String(UART1,0,prnt_bffr);
	}
	
	
	//Delay
	timeout = 100;
	while(timeout != 0){
		timeout--;
	}
	
	// CONFIG2 init
	index=0;
	do
	{
		array_name[0]=pgm_read_byte(&CONFIG2[index]);  // internal reg. addr
		index++;
		array_name[1]=pgm_read_byte(&CONFIG2[index]);  // value for the reg.
		index++;
		timeout=50;
		do
		{
			status=TWI_Master_Transmit(TWI1_Base,0x43,2,array_name);
			timeout--;
		}while((status!=no_errors)&&(timeout!=0));
	} while((array_name[0]!=0xFF)&&(timeout!=0));
	if(timeout!=0){
		sprintf(prnt_bffr,"Config2 sent...\n\r");
		UART_Transmit_String(UART1,0,prnt_bffr);
	}
	
}