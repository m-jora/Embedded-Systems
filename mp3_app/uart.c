#include "uart.h"

void UART_Init(volatile UART_t * UART_addr, uint32_t Baud_Rate){
	uint16_t UBRR_value = (uint16_t)((((F_CPU / OSC_DIV) + (4UL * (2-U2X) * Baud_Rate)) / (8UL * (2 - U2X) * Baud_Rate)) -1);
	
	(UART_addr->UART_UBRRH) = UBRR_value/256;
	(UART_addr->UART_UBRRL) = UBRR_value%256;
	
	(UART_addr->UART_UCSRA) = TXC|MPCM|U2XA;
	(UART_addr->UART_UCSRB) = (Interrupt_Disabled|IO_Enable|One_Stop_Bit_2|No_Ninth_Bit);
	(UART_addr->UART_UCSRC) = (Async_Mode|No_Parity|One_Stop_Bit|Eight_Bit_Data);
}

void UART_Transmit(volatile UART_t * UART_addr, int8_t send_value){
	do{
		;
	}while(!(UDRE & UART_addr->UART_UCSRA));
	UART_addr->UART_UDR = send_value;
}

uint8_t UART_Receive(volatile UART_t * UART_addr){
	do{
		;
	}while(!(RXC & UART_addr->UART_UCSRA));
	uint8_t ret_val = UART_addr->UART_UDR;
	return ret_val;
}
