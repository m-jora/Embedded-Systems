#include <avr/io.h>
#include "board_struct.h"
#include "TWI.h"
#include "STA013_Config.h"
#include <stdio.h>
#include "UART.h"
#include "UART_Print.h"
#include <avr/pgmspace.h>
#include "GPIO_Outputs.h"
#include "LEDS.h"

extern const uint8_t CONFIG[3998];
extern const uint8_t CONFIG2[50];


/***********************************************************************
DESC:    Initializes the STA013 with the following steps:
         Applies a hardware reset signal, checks the ID register for an 0xAC
		 response and then sends the configuration data.
RETURNS: error flag
CAUTION: 
          
************************************************************************/
const uint8_t STA_cfg_addr[18] PROGMEM = {0x54, 0x55,  6, 11,  82,  81, 101, 100, 80, 97, 0x05, 0x0d, 0x18, 0x0c, 0x46, 0x48, 0x7d, 0x72};
const uint8_t STA_cfg_data[18] PROGMEM = {0x07, 0x10,  9,  2, 184,   0,   0,   0,  6,  5, 0xa1, 0x00, 0x04, 0x05, 0x07, 0x07, 0x00, 0x01};

const char Error_prnt[13] PROGMEM = {"error flag: "};
const char ID_Reg_prnt[26] PROGMEM = {"Output from ID register: "};
const char STA_Ver_prnt[21] PROGMEM = {"STA013 SoftVersion: "};
const char First_config_prnt[35] PROGMEM = {"First configuration file sent...\n\r"};
const char Second_config_prnt[36] PROGMEM = {"Second configuration file sent...\n\r"};
const char Number_written_prnt[27] PROGMEM = {"Number of values written: "};
const char STA_sent_prnt[17] PROGMEM = {"Sent to STA013: "};
const char STA_rec_prnt[21] PROGMEM = {"Output from STA013: "};
const char Config_Complete_prnt[25] PROGMEM = {"Configuration Complete\n\r"};
	
uint8_t STA013_Init(void)
{
   uint8_t error_flag, timeout, length, reg_addr;
   uint8_t send[2], rcv[3];
   uint16_t j,k;
   char * prnt_bffr;
   
   /********* Reset the STA013 **********/
   GPIO_Output_Clear(STA013_Reset_Port,STA013_Reset_Pin);
   GPIO_Output_Init(STA013_Reset_Port,STA013_Reset_Pin);  
   timeout=50;
   prnt_bffr=Export_print_buffer();
   GPIO_Output_Set(STA013_Reset_Port,STA013_Reset_Pin);
 /****** Check for 0xAC in the STA013 ID register Version 1 *******/
      timeout=50;
	  do
	  {
        error_flag=TWI_Master_Receive(STA013_TWI_Port,STA013,1,0,3,rcv);
        timeout--;
	  }while((error_flag!=no_errors)&&(timeout!=0));
	  /****** Print the value read from the STA013 ID register *******/
	  if(timeout!=0)
	  {
         length = Copy_String_to_Buffer(ID_Reg_prnt,0,prnt_bffr);
         sprintf((prnt_bffr+(length-1)), "%2.2X \n\r",rcv[2]);
		 UART_Transmit_String(UART1,0,prnt_bffr);
	  }
	  else  // Print error flag for debugging
	  {
		  length = Copy_String_to_Buffer(Error_prnt,0,prnt_bffr);
		  sprintf((prnt_bffr+(length-1)), "%2.2X \n\r",error_flag);
		  UART_Transmit_String(UART1,0,prnt_bffr);
	  } 

   
/****** Check for 0xAC in the STA013 ID register Version 2 *******/
	  timeout=50;
	  do
	  {
		  error_flag=TWI_Master_Receive(STA013_TWI_Port,STA013,1,1,1,rcv);
		  timeout--;
	  }while((error_flag!=no_errors)&&(timeout!=0));
	  /****** Print the value read from the STA013 ID register *******/
	  if(timeout!=0)
	  {
		  length = Copy_String_to_Buffer(ID_Reg_prnt,0,prnt_bffr);
		  sprintf((prnt_bffr+(length-1)), "%2.2X \n\r",rcv[0]);
		  UART_Transmit_String(UART1,0,prnt_bffr);
	  }
	  else  // Print error flag for debugging
	  {
		  length = Copy_String_to_Buffer(Error_prnt,0,prnt_bffr);
		  sprintf((prnt_bffr+(length-1)), "%2.2X \n\r",error_flag);
		  UART_Transmit_String(UART1,0,prnt_bffr);
	  }

   //Debugging LED output
   LEDS_On(LED1_port,LED1_pin);
   if(rcv[0]==0xAC)
   {
     /****** Send the first configuration section *******/
     j=0;
	 k=0;
     do
     {
        reg_addr=pgm_read_byte(&CONFIG[j]);
        j++;
        send[0]=pgm_read_byte(&CONFIG[j]);
        j++;
        if((reg_addr!=0xff)&&(send[0]!=0xFF))
        {
          
           timeout=50;
           do
           {
               error_flag=TWI_Master_Transmit(STA013_TWI_Port,STA013,reg_addr,1,1,send);
               timeout--;
               if(timeout==0)
			   {
				    LEDS_On(LED0_port,LED0_pin);
					sprintf(prnt_bffr,"%u\n\r",error_flag);
					UART_Transmit_String(UART1,0,prnt_bffr);
			   }
           }while((error_flag!=no_errors)&&(timeout!=0));
		   
        }
		if((error_flag==no_errors)&&(timeout!=0))
		{
			k++;
		}
     }while(((reg_addr!=0xff)||(send[0]!=0xff)) && (timeout!=0));
	 // Debugging output to indicated first configuration section sent
	 Copy_String_to_Buffer(First_config_prnt,0,prnt_bffr);
	 UART_Transmit_String(UART1,0,prnt_bffr);
	 length = Copy_String_to_Buffer(Number_written_prnt,0,prnt_bffr);
	 sprintf((prnt_bffr+(length-1)), "%2.2u \n\r",k);
	 UART_Transmit_String(UART1,0,prnt_bffr);
	 // Debugging LED output 
	 LEDS_On(LED2_port,LED2_pin);
     j=0;
	 k=0;
     /****** Send the second configuration section *******/
     do
     {
        reg_addr=pgm_read_byte(&CONFIG2[j]);
        j++;
        send[0]=pgm_read_byte(&CONFIG2[j]);
        j++;
        if((reg_addr!=0xff)&&(send[0]!=0xFF))
        {
           timeout=50;
           do
           {
               error_flag=TWI_Master_Transmit(STA013_TWI_Port,STA013,reg_addr,1,1,send);
               timeout--;
           }while((error_flag!=no_errors)&&(timeout!=0));
        }
		if((error_flag==no_errors)&&(timeout!=0))
		{
			k++;
		}
     }while(((reg_addr!=0xFF)||(send[0]!=0xFF)) && (timeout!=0) && (error_flag==no_errors));
	 Copy_String_to_Buffer(Second_config_prnt,0,prnt_bffr);
	 UART_Transmit_String(UART1,0,prnt_bffr);
	 length = Copy_String_to_Buffer(Number_written_prnt,0,prnt_bffr);
	 sprintf((prnt_bffr+(length-1)), "%2.2u \n\r",k);
	 UART_Transmit_String(UART1,0,prnt_bffr);
     // Debugging LED output 
	 LEDS_On(LED3_port,LED3_pin); 
	 /****** Send the third configuration section *******/ 
     for(j=0;j<18;j++)
	  {
	    reg_addr=pgm_read_byte(&STA_cfg_addr[j]);
	    send[0]=pgm_read_byte(&STA_cfg_data[j]);
        timeout=50;
        do
        {
            error_flag=TWI_Master_Transmit(STA013_TWI_Port,STA013,reg_addr,1,1,send);
            timeout--;
            if(timeout==0) LEDS_On(LED0_port,LED0_pin);
        }while((error_flag!=no_errors)&&(timeout!=0));
		/******* Print the values written from the thrid configuration section to verify the values *******/
		length = Copy_String_to_Buffer(STA_sent_prnt,0,prnt_bffr);
		sprintf((prnt_bffr+(length-1)), "%2.2X  %2.2X \n\r",reg_addr,send[0]);
		UART_Transmit_String(UART1,0,prnt_bffr);
		error_flag=TWI_Master_Receive(STA013_TWI_Port,STA013,reg_addr,1,1,rcv);
		length = Copy_String_to_Buffer(STA_rec_prnt,0,prnt_bffr);
		sprintf((prnt_bffr+(length-1)), "%2.2X  %2.2X %2.2X \n\r",reg_addr,rcv[0],error_flag);
		UART_Transmit_String(UART1,0,prnt_bffr);

	  }
	  Copy_String_to_Buffer(Config_Complete_prnt,0,prnt_bffr);
	  UART_Transmit_String(UART1,0,prnt_bffr);
   }   
   // Switch off debugging LEDs
   LEDS_Off(LED1_port,LED1_pin);
   LEDS_Off(LED2_port,LED2_pin);
   LEDS_Off(LED3_port,LED3_pin);  
   return error_flag;
}
