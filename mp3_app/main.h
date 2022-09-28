#ifndef	_MAIN_H
#define _MAIN_H

#include <stdint.h>

#define F_CPU (16000000)
#define OSC_DIV (1)
//#define OSC_F(F_CPU / OSC_DIV)
//#define baud_rate ((OSC_F / (8 * (2 - U2X))
//#define UBRR_value((OSC_F / (8 * (2 - U2X) * baud_rate)) -1)


#define PIN_offset (0)
#define DDR_offset (1)
#define PORT_offset (2)

#define PC ((volatile GPIO_t *)(&PINC))
#define PB ((volatile GPIO_t *)(&PINB))
#define PE ((volatile GPIO_t *)(&PINE))
#define PA ((volatile GPIO_t *)(&PINA))

#define UART0 ((volatile UART_t *)(0xC0))
#define UART1 ((volatile UART_t *)(0xC8))
#define UART2 ((volatile UART_t *)(0xD0))


/*
1.	Done
2.	Done
3.	Done
4.	Done
5.	
6.	
7.	
8.	
9.	
10.	
11.	
12.	
*/

typedef struct GPIO{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;

typedef struct UART{
	volatile uint8_t UART_UCSRA;
	volatile uint8_t UART_UCSRB;
	volatile uint8_t UART_UCSRC;
	volatile uint8_t UART_UCSRD;
	volatile uint8_t UART_UBRRL;
	volatile uint8_t UART_UBRRH;
	volatile uint8_t UART_UDR;
	} UART_t;

#endif