#ifndef	_UART_H
#define _UART_H

#include "main.h"

//UART_UCSRC
#define Async_Mode (0<<6)
#define No_Parity (0<<4)
#define One_Stop_Bit (0<<3)
#define Eight_Bit_Data (1<<1)|(1<<2)

void UART_Init(volatile UART_t * UART_addr, uint32_t Baud_Rate);


#endif