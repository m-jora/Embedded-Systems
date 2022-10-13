#include <stdio.h>
#include "board_struct.h"
#include "Long_Serial_In.h"
#include <stdlib.h>
#include "UART.h"


/***********************************************************************
DESC:    Waits for user input of a long integer as ASCII characters
        
INPUT: Nothing
RETURNS: unsigned long
CAUTION: UART must be initialized first
************************************************************************/

uint32_t Long_Serial_Input(volatile UART_t * UART_addr)
{
   uint8_t index,input;
   char input_values[11];   
   uint32_t output_value;
      for(index=0;index<11;index++)
      {
        input_values[index]=0;
      }
      output_value=0xffffffff;
      index=0;
      do
      {
         input=UART_Receive(UART_addr);
         if((input>=0x30)&&(input<=0x39))
         {
           UART_Transmit(UART_addr,input);    // echo valid inputs only
		   input_values[index]=input;
           index++;
         }
         else if ((input==BS)||(input==DEL))  // Putty uses DEL (127) or BS (0x08) for backspace
         {
           index--;
           input_values[index]=0;
		   UART_Transmit(UART_addr,BS);
           UART_Transmit(UART_addr,space);
           UART_Transmit(UART_addr,BS);
         }
		 //*** Added '.' as a replacement for ENTER since CR or LF does not work for Atmel Studio Data Visualizer ****//
         else if ((input==CR)||(input==LF)||(input=='.'))
         {
            output_value=atol(input_values);
            UART_Transmit(UART_addr,CR);
			UART_Transmit(UART_addr,LF);
         }
         if(index==10)
         {
            output_value=atol(input_values);
            UART_Transmit(UART_addr,CR);
			UART_Transmit(UART_addr,LF);
         }
      }while(output_value==0xffffffff);

return output_value;
}
