
#include <avr/io.h>
#include "board_struct.h"
#include "UART.h"
#include <avr/pgmspace.h>

/****** Private Constants *******/
#define U2X_bit (0)  // 0 - regular baud rate, 1 - double baud rate
/****** Register Bit Settings *****/
/****** UCSRA bits *******/
#define MPCM_shift (0)
#define U2X_shift (1)
#define TXC_shift (6)
/****** UCSRB bits *******/
#define TXB8_shift (0)
#define Nine_Data_Bits_B (1<<2)
#define TX_enable (1<<3)
#define RX_enable (1<<4)
#define UDRE_interrupt_enable (1<<5)
#define TX_interrupt_enable (1<<6)
#define RX_interrupt_enable (1<<7)
/****** UCSRC bits *******/
#define Asynchronous (0<<6)
#define Synchronous (1<<6)
#define Master_SPI (3<<6)
#define No_Parity (0<<4)
#define Even_Parity (2<<4)
#define Odd_Parity (3<<4)
#define One_Stop_Bit (0<<3)
#define Two_Stop_Bits (1<<3)
#define Five_Data_Bits (0<<1)
#define Six_Data_Bits (1<<1)
#define Seven_Data_Bits (2<<1)
#define Eight_Data_Bits (3<<1)
#define Nine_Data_Bits_C (3<<1)
#define SPI_MSB_First (0<<2)
#define SPI_LSB_First (1<<2)
#define SPI_MSB_First (0<<2)
#define SPI_CPHA_Leading_Edge (0<<1)
#define SPI_CPHATrailing_Edge (1<<1)
#define SPI_CPOL_0 (0<<0)
#define SPI_CPOL_1 (1<<0)




void UART_Init (volatile UART_t * UART_addr, uint16_t Baud_Rate)
{
	uint16_t Baud_Rate_Reload;
	Baud_Rate_Reload = (uint16_t)(((F_CPU/F_DIV)/(8UL*(2-U2X_bit)*Baud_Rate))-1);
	
	(UART_addr->UBBRH) = (Baud_Rate_Reload / 256);
	(UART_addr->UBBRL) = (Baud_Rate_Reload % 256);
		
	(UART_addr->UCSRA) = U2X_bit<<U2X_shift;
	(UART_addr->UCSRC) = Asynchronous|No_Parity|One_Stop_Bit|Eight_Data_Bits;
	(UART_addr->UCSRB) = RX_enable|TX_enable;
}

char UART_Transmit(volatile UART_t * UART_addr, char c)
{
	uint8_t status;
	do
	{
		status=(UART_addr->UCSRA);
	} while ((status & (1 << UDRE))!=(1 << UDRE));
	(UART_addr->UDR) = c;
	return 0;
}


char UART_Receive(volatile UART_t * UART_addr)
{
	uint8_t status;
	do 
	{
		status=(UART_addr->UCSRA);
	} while ((status & (1 << RXC))!=(1 << RXC));
	return (UART_addr->UDR);  // return the value in the receive buffer
}

