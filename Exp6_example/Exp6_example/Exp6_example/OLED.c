/*
 * CFile1.c
 *
 * Created: 7/1/2020 3:45:29 PM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "SPI.h"
#include "GPIO_Outputs.h"
#include <avr/pgmspace.h>
#include "OLED.h"


/* The initialization command codes for the OLED display (128x32 display). */
const  uint8_t OLED_Init_Commands[24] PROGMEM = {0xAE, 0xD5, 0x80, 0xA8, 0x1F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0xA1, 0xC8, 0xDA, 0x02, 0x81, 0x8F, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF, 0xFF};
/* Refers to the character map file "OLED_Char_Map.c" */
extern const uint8_t OLED_ASCII[127][5];
									
									

void OLED_Init(volatile SPI_t *OLED_port)
{
	uint16_t index;
	//uint8_t * commands_p;
	uint8_t command,temp8;
	GPIO_Output_Set(OLED_Reset_Port,OLED_Reset_Pin);
	GPIO_Output_Init(OLED_Reset_Port,OLED_Reset_Pin); //OLED_Reset_Init();
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin);
	GPIO_Output_Init(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Init();
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin);
	GPIO_Output_Init(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Init();
	// Reset
	GPIO_Output_Clear(OLED_Reset_Port,OLED_Reset_Pin); //OLED_Reset_Control(0);
	for(index=0;index<500;index++);
	GPIO_Output_Set(OLED_Reset_Port,OLED_Reset_Pin); //OLED_Reset_Control(1);
	// loop to send initialization commands
	GPIO_Output_Clear(OLED_CMD_Port,OLED_CMD_Pin);// OLED_CMD_Control(0);  // All SPI values send will be commands
	//commands_p=OLED_Init_Commands;
	index=0;
	command=pgm_read_byte(&OLED_Init_Commands[index]);//*commands_p;
	
	while(command!=0xFF)
	{
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		temp8=SPI_Transmit(OLED_port, command);
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
		index++;
		command=pgm_read_byte(&OLED_Init_Commands[index]);//*(commands_p+index);
	}
	// Clear Display by filling each line (page) with all 0x00's
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x40);            //Set cursor to home
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	OLED_Clear_Display(OLED_port);
	
}

/*********************************************************************************
*          The following function are used to position the cursor
*          The OLED_Set_Line_X functions are used to place the cursor
*              at the start of the specified line (page).  If only the line (page)
*              number is sent, then the cursor moves to that line, but does not
*              reset back to the start of the line (like a LF without a CR).
*
*          The OLED_Set_Cursor function can be used to place the cursor at any point
*              (in pixels) within a line (page).  The character width is 6 pixels.
*              Setting the cursor to position 0 is the same as a CR without a LF. 
**********************************************************************************/ 

/**********************************************************************************
*          Sets the cursor to the start of line 0 in page mode 
***********************************************************************************/
void OLED_Set_Line_0 (volatile SPI_t *OLED_port)
{
	uint8_t temp8;
	GPIO_Output_Clear(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(0);  // All SPI values send will be commands
	//Sets to line 0 (page 0)
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0xB0);  // Page 0
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	//Sets the cursor address to 0x00 (start of line (page))  
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x00);   // least significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x10);   // most significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
}

/**********************************************************************************
*          Sets the cursor to the start of line 1 in page mode
***********************************************************************************/
void OLED_Set_Line_1 (volatile SPI_t *OLED_port)
{
	uint8_t temp8;
	GPIO_Output_Clear(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(0);  // All SPI values send will be commands
	//Sets to line 1 (page 1)
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0xB1);  // Page 1
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	//Sets the cursor address to 0x00 (start of line (page))
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x00);    // least sig. nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x10);    // most sig. nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1); 
}

/**********************************************************************************
*          Sets the cursor to the start of line 2 in page mode
***********************************************************************************/
void OLED_Set_Line_2 (volatile SPI_t *OLED_port)
{
	uint8_t temp8;
	GPIO_Output_Clear(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(0);  // All SPI values send will be commands
	//Sets to line 2 (page 2)
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0xB2);  // Page 2
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	//Sets the cursor address to 0x00 (start of line (page))
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x00);    // least significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x10);    // most significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1); 
}

/**********************************************************************************
*          Sets the cursor to the start of line 3 in page mode
***********************************************************************************/
void OLED_Set_Line_3 (volatile SPI_t *OLED_port)
{
	uint8_t temp8;
	GPIO_Output_Clear(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(0);  // All SPI values send will be commands
	//Sets to line 3 (page 3)
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0xB3);  // Page 3
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	//Sets the cursor address to 0x00 (start of line (page))
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x00);      // least significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x10);      // most significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
}

/**********************************************************************************
*          Sets the cursor to the specified position (0 to 127) in a page (line).
***********************************************************************************/
void OLED_Set_Cursor (volatile SPI_t *OLED_port, uint8_t position)
{
	uint8_t temp8;
	GPIO_Output_Clear(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(0);  // All SPI values send will be commands
	temp8=(position&0x0F);
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, (0x00|temp8));    // least significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	temp8=(position>>4);
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, (0x10|temp8));    // most significant nibble of cursor address
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
}

/************************************************************************************
*         Sends one ASCII character to the OLED display connected to the specified SPI
*         The pixels for a character are defined in the file "OLED_Char_Map.c"
*         A '1' is on, a '0' is off.  The pixels are numbered, msb bottom to lsb top.
*         The character map defines 5x7 characters to leave a space at the top (lsb is '0').
*         One column of pixels is placed after the character for spacing. 
*************************************************************************************/

void OLED_Send_Char (volatile SPI_t *OLED_port, char ASCII_input)
{
	uint8_t temp8, output;
	uint8_t index, i;
	index=(ASCII_input);
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(1);  // Send Data
	for(i=0;i<5;i++)
	{
		output=pgm_read_byte(&OLED_ASCII[index][i]);
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		temp8=SPI_Transmit(OLED_port, output);  // Write pixels
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	}
	GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
	temp8=SPI_Transmit(OLED_port, 0x00); // One row of pixels following character
	GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
}

/**********************************************************************************
*         Clears the current line (0 to 3).  Use OLED_Set_LineX to specify line to clear.
***********************************************************************************/
void OLED_Clear_Line(volatile SPI_t *OLED_port)
{
	uint8_t index;
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(1);  // Write data
	for(index=0;index<128;index++)
	{
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		SPI_Transmit(OLED_port, 0x00);
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	}
}

/**********************************************************************************
*         Clears all four lines of the display.
***********************************************************************************/
void OLED_Clear_Display(volatile SPI_t *OLED_port)
{
    uint8_t index;
	OLED_Set_Line_0 (OLED_port);  
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(1);  // Write data// Write data
	for(index=0;index<128;index++)
	{
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		SPI_Transmit(OLED_port, 0x00);
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	}
	OLED_Set_Line_1 (OLED_port);
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(1);  // Write data
	for(index=0;index<128;index++)
	{
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		SPI_Transmit(OLED_port, 0x00);
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	}
	OLED_Set_Line_2 (OLED_port);
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(1);  // Write data
	for(index=0;index<128;index++)
	{
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		SPI_Transmit(OLED_port, 0x00);
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	}
	OLED_Set_Line_3 (OLED_port);
	GPIO_Output_Set(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Control(1);  // Write data
	for(index=0;index<128;index++)
	{
		GPIO_Output_Clear(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(0);
		SPI_Transmit(OLED_port, 0x00);
		GPIO_Output_Set(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Control(1);
	}
}

/*********************************************************************************************
*           Transmits a string of characters to the OLED display using the OLED_Send_Char function
*           The SPI port for the OLED display must be specified as a volatile pointer.
*           If the number of bytes to print is 0, then the string is printed until a NULL is found,
*           otherwise the specified number of bytes is printed.
***********************************************************************************************/ 
void OLED_Transmit_String(volatile SPI_t *OLED_port, uint8_t num_bytes,char * string_name)
{
	int8_t temp8,index;
	if(num_bytes==0)
	{
		temp8=*string_name;
		while (temp8!=0)
		{
			OLED_Send_Char(OLED_port, temp8);
			string_name++;
			temp8=*string_name;
		}
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			OLED_Send_Char(OLED_port, string_name[index]);
		}
	}
}
	
	
	