#include "uart.h"

void UART_Init(volatile UART_t * UART_addr, uint32_t Baud_Rate){
	uint32_t OSC_F = F_CPU / OSC_DIV;
	uint16_t UBRR_value = (uint16_t)((OSC_F + (4UL * (2-U2X) * Baud_Rate)) / (8UL * (2 - U2X) * Baud_Rate)) -1;
	
	(UART_addr->UART_UBRRH) = UBRR_value/256;
	(UART_addr->UART_UBRRL) = UBRR_value%256;
	
	(UART_addr->UART_UCSRA) = (0<<6)|(0<<0);
	//(UART_addr->UART_UCSRB) = //UCSZ2 needs to be 0 in here
	(UART_addr->UART_UCSRC) = (Async_Mode|No_Parity|One_Stop_Bit|Eight_Bit_Data);
	
}