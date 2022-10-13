
#ifndef  _UART_H
#define  _UART_H



/***** Public Function prototypes ********/

void UART_Init (volatile UART_t * UART_addr, uint16_t Baud_Rate);
char UART_Transmit(volatile UART_t * UART_addr, char c);
char UART_Receive(volatile UART_t * UART_addr);


#endif