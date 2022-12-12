#include <stdio.h>
#include "board_struct.h"
#include "UART.h"
#include "UART_Print.h"
#include "print_memory.h"
#include <avr/pgmspace.h>

/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A)

const char Prnt_bytes_prnt[64] PROGMEM = {"Addr. Offset   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n\r"}; 

/****** Private Functions **************/

void print_16bytes(volatile UART_t * UART_addr, uint8_t * array_in, uint16_t offset);

/***********************************************************************
DESC:    Prints 8 bytes in Hexadecimal and then ASCII
INPUT: Pointer to an array of eight bytes
RETURNS: nothing
CAUTION: 
************************************************************************/
void print_16bytes(volatile UART_t * UART_addr, uint8_t * array_in, uint16_t offset)
{
   uint8_t * input;
   uint8_t index, dat;
   char * prnt_bffr;
   
   prnt_bffr=Export_print_buffer();
   
   input = array_in+offset;
   sprintf(prnt_bffr, "%p ",input);
   UART_Transmit_String(UART_addr,0,prnt_bffr);
   sprintf(prnt_bffr,"0x%4.4X  ",offset);
   UART_Transmit_String(UART_addr,0,prnt_bffr);
   for (index=0;index<16;index++)
   {
      dat=*(input+index);
      sprintf(prnt_bffr, "%2.2X ",dat);   // Changed from %2.2bX because this printf function can correctly determine number of bytes
	  UART_Transmit_String(UART_addr,0,prnt_bffr);
   }
   for (index=0;index<16;index++)
   { 
      dat=*(input+index);
      if (dat<32 || dat>127) dat=46;
      UART_Transmit(UART_addr,dat);
   }
   UART_Transmit(UART_addr,CR);
   UART_Transmit(UART_addr,LF);
}


/***********************************************************************
DESC: Prints an array from memory in Hexadecimal and then ASCII
INPUT: Pointer to an array, number of bytes to print
RETURNS: nothing
CAUTION: 
************************************************************************/
void print_memory(volatile UART_t * UART_addr, uint16_t number_of_bytes, uint8_t * array_in)
{
   uint16_t input;
   char * prnt_bffr;
   
   prnt_bffr=Export_print_buffer();
   
   input = 0;
   Copy_String_to_Buffer(Prnt_bytes_prnt,0,prnt_bffr);
   UART_Transmit_String(UART_addr,0,prnt_bffr);
   do
   {
      print_16bytes(UART_addr,array_in,input);
      input+=16;
   }while(input<(number_of_bytes));
}
		