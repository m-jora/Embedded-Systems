#include "uart.h"

void UART_Init(volatile UART_t * UART_addr, uint32_t Baud_Rate){
	uint32_t OSC_F = F_CPU / OSC_DIV;
	uint16_t UBRR_value = (uint16_t)((OSC_F + (4UL * (2-U2X) * Baud_Rate)) / (8UL * (2 - U2X) * Baud_Rate)) -1;
	
	(UART_addr->UART_UBRRH) = UBRR_value/256;
	(UART_addr->UART_UBRRL) = UBRR_value%256;
	
	(UART_addr->UART_UCSRA) = TXC|MPCM|U2X;
	(UART_addr->UART_UCSRB) = (Interrupt_Disabled|IO_Enable|One_Stop_Bit_2|No_Ninth_Bit);
	(UART_addr->UART_UCSRC) = (Async_Mode|No_Parity|One_Stop_Bit|Eight_Bit_Data);
}

int8_t UART_Transmit(volatile UART_t * UART_addr, int8_t send_value){
	do{
		;
	}while(!(UDRE && UART_addr->UART_UCSRA));
		
	UART_addr->UART_UDR = send_value;
	return 0;
}

uint8_t UART_Receive(volatile UART_t * UART_addr){
	do{
		;
	}while(!(RXC && UART_addr->UART_UCSRA));
	return UART_addr->UART_UDR;
}
