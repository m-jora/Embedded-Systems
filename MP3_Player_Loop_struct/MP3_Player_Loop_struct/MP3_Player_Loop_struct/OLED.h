/*
 * OLED.h
 *
 * Created: 7/1/2020 4:30:27 PM
 *  Author: youngerr
 */ 


#ifndef OLED_H_
#define OLED_H_

/****** OLED IO Definitions *******/
#define OLED_SPI_Port (SPI0)  // Defines which SPI port is used for SD Card communication
#define OLED_SS_Port (PB)  // Defines the GPIO Port for OLED Slave Select
#define OLED_SS_Pin (1<<0) // Defines the GPIO Pin for OLED Slave Select
#define OLED_Reset_Port (PD)  // Defines the GPIO Port for OLED Reset
#define OLED_Reset_Pin (1<<7) // Defines the GPIO Pin for OLED Reset
#define OLED_CMD_Port (PA)  // Defines the GPIO Port for OLED Slave Select
#define OLED_CMD_Pin (1<<6) // Defines the GPIO Pin for OLED Slave Select


void OLED_Init(volatile SPI_t *OLED_port);
void OLED_Set_Line_0 (volatile SPI_t *OLED_port);
void OLED_Set_Line_1 (volatile SPI_t *OLED_port);
void OLED_Set_Line_2 (volatile SPI_t *OLED_port);
void OLED_Set_Line_3 (volatile SPI_t *OLED_port);
void OLED_Set_Cursor (volatile SPI_t *OLED_port, uint8_t position);
void OLED_Send_Char (volatile SPI_t *OLED_port, char ASCII_input);
void OLED_Clear_Line(volatile SPI_t *OLED_port);
void OLED_Clear_Display(volatile SPI_t *OLED_port);
void OLED_Transmit_String(volatile SPI_t *OLED_port, uint8_t num_bytes,char * string_name);


#endif /* OLED_H_ */