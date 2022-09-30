#ifndef	_UART_H
#define _UART_H

#include "main.h"

#define U2X (0)

//UART_UCSRA
#define TXC (0<<6)
#define MPCM (0<<0)
#define U2XA (0<<0)

//UART_UCSRB
#define Interrupt_Disabled (0<<7)|(0<<6)|(0<<5)
#define IO_Enable (1<<4)|(1<<3)
#define One_Stop_Bit_2 (0<<2)
#define No_Ninth_Bit (0<<0)|(0<<1)

//UART_UCSRC
#define Async_Mode (0<<6)
#define No_Parity (0<<4)
#define One_Stop_Bit (0<<3)
#define Eight_Bit_Data (1<<1)|(1<<2)

#define UDRE (1<<5)
#define RXC (1<<7)

void UART_Init(volatile UART_t * UART_addr, uint32_t Baud_Rate);
void UART_Transmit(volatile UART_t * UART_addr, int8_t send_value);
uint8_t UART_Receive(volatile UART_t * UART_addr);

#endif
