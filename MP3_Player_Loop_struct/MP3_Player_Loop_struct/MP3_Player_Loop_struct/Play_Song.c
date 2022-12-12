#include <stdio.h>
#include "board_struct.h"
#include "SPI.h"
#include "File_System_struct.h"
#include "Read_Sector.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"
#include "Read_Inputs.h"
#include "STA013_Config.h"
#include "UART_Print.h"
#include "Play_Song.h"
#include "Directory_Functions_struct.h"
extern uint8_t buffer1_g[512];
extern uint8_t buffer2_g[512];

void Play_Song(uint32_t Start_Cluster)
{
	uint16_t index1, index2;
	uint8_t buf_flag1, buf_flag2, temp8;
	uint32_t sector, sector_offset;
	char *prnt_bffr;
	FS_values_t * Drive_p = Export_Drive_values();

	prnt_bffr=Export_print_buffer();
	//**** LEDS initialized for debugging ****//
	LEDS_Off(LED3_port,LED3_pin);
	LEDS_Init(LED3_port,LED3_pin);
	LEDS_Off(LED1_port,LED1_pin);
	LEDS_Init(LED1_port,LED1_pin);
	LEDS_Off(LED0_port,LED0_pin);
	LEDS_Init(LED0_port,LED0_pin);
	LEDS_Off(LED2_port,LED2_pin);
	LEDS_Init(LED2_port,LED2_pin);
	GPIO_Output_Clear(STA013_BITEN_Port,STA013_BITEN_Pin);
	GPIO_Output_Init(STA013_BITEN_Port,STA013_BITEN_Pin);
	GPIO_Pull_Up_Enable(STA013_DATAREQ_Port,STA013_DATAREQ_Pin);
	GPIO_Input_Init(STA013_DATAREQ_Port,STA013_DATAREQ_Pin);
	sprintf(prnt_bffr,"Starting Cluster = %lu\n\r",Start_Cluster);
	UART_Transmit_String(UART1,0,prnt_bffr);
	sector=First_Sector(Start_Cluster);
	sprintf(prnt_bffr,"Starting Sector = %lu\n\r",sector);
	UART_Transmit_String(UART1,0,prnt_bffr);
	sector_offset=0;
	buf_flag1=1;
	buf_flag2=0;
	//P3_2=ON;
	LEDS_On(LED1_port,LED1_pin);
	index1=0;
	
	Read_Sector(sector+sector_offset, 512, buffer1_g);

	sector_offset++;
	LEDS_Off(LED1_port,LED1_pin);
	LEDS_On(LED2_port,LED2_pin);
	index2=0;
	
	Read_Sector(sector+sector_offset, 512, buffer2_g);
	sector_offset++;
	LEDS_Off(LED2_port,LED2_pin);
	do
	{
		do
		{
			temp8=GPIO_Read_Pin(STA013_DATAREQ_Port,STA013_DATAREQ_Pin);
			if(temp8==0)
			{
				//GREENLED=ON;
				GPIO_Output_Set(STA013_BITEN_Port,STA013_BITEN_Pin);
				SPI_Transmit(STA013_SPI_Port,buffer1_g[index1]);
				GPIO_Output_Clear(STA013_BITEN_Port,STA013_BITEN_Pin);
				//GREENLED=OFF;
				index1++;
				if(index1>511)
				{
					if(index2>511)
					{
						//BIT_EN=0;
						LEDS_On(LED2_port,LED2_pin);
						index2=0;
						
						// If end of sector reached, get next cluster
						if (sector_offset >= (Drive_p->SecPerClus)){
							// LED for debugging
							LEDS_On(LED3_port, LED3_pin);
							Start_Cluster = Find_Next_Clus(Start_Cluster, buffer2_g);
							LEDS_Off(LED3_port, LED3_pin);
							sector = First_Sector(Start_Cluster);
							// Reset offset
							sector_offset = 0;
						}
						
						// Fill buffer2
						Read_Sector(sector+sector_offset, 512, buffer2_g);
						sector_offset++;
						
						LEDS_Off(LED2_port,LED2_pin);
					}
					buf_flag1=0;
					buf_flag2=1;

				}
			}
			else
			{
				if(index2>511)
				{
					//BIT_EN=0;
					LEDS_On(LED2_port,LED2_pin);
					index2=0;
					
					// If end of sector reached, get next cluster
					if (sector_offset >= (Drive_p->SecPerClus)){
						// LED for debugging
						LEDS_On(LED3_port, LED3_pin);
						Start_Cluster = Find_Next_Clus(Start_Cluster, buffer2_g);
						LEDS_Off(LED3_port, LED3_pin);
						sector = First_Sector(Start_Cluster);
						// Reset offset
						sector_offset = 0;
					}
					
					// Fill buffer2
					Read_Sector(sector+sector_offset, 512, buffer2_g);
					sector_offset++;
					
					LEDS_Off(LED2_port,LED2_pin);
				}
				else
				{
					if(index1>511)
					{
						buf_flag1=0;
						buf_flag2=1;
					}
				}
			}
		}while(buf_flag1==1);
		do
		{
			temp8=GPIO_Read_Pin(STA013_DATAREQ_Port,STA013_DATAREQ_Pin);
			if(temp8==0)
			{
				//REDLED=ON;
				GPIO_Output_Set(STA013_BITEN_Port,STA013_BITEN_Pin);
				SPI_Transmit(STA013_SPI_Port,buffer2_g[index2]);
				GPIO_Output_Clear(STA013_BITEN_Port,STA013_BITEN_Pin);
				//REDLED=OFF;
				index2++;
				if(index2>511)
				{
					if(index1>511)
					{
						//BIT_EN=0;
						LEDS_On(LED1_port,LED1_pin);
						index1=0;
						
						// If end of sector reached, get next cluster
						if (sector_offset >= (Drive_p->SecPerClus)){
							// LED for debugging
							LEDS_On(LED3_port, LED3_pin);
							Start_Cluster = Find_Next_Clus(Start_Cluster, buffer1_g);
							LEDS_Off(LED3_port, LED3_pin);
							sector = First_Sector(Start_Cluster);
							// Reset offset
							sector_offset = 0;
						}

						// Fill buffer1
						Read_Sector(sector+sector_offset, 512, buffer1_g);
						sector_offset++;
						
						LEDS_Off(LED1_port,LED1_pin);
					}
					buf_flag2=0;
					buf_flag1=1;
					
				}
			}
			else
			{
				if(index1>511)
				{
					//BIT_EN=0;
					LEDS_On(LED1_port,LED1_pin);
					index1=0;
					
					// If end of sector reached, get next cluster
					if (sector_offset >= (Drive_p->SecPerClus)){
						// LED for debugging
						LEDS_On(LED3_port, LED3_pin);
						Start_Cluster = Find_Next_Clus(Start_Cluster, buffer1_g);
						LEDS_Off(LED3_port, LED3_pin);
						sector = First_Sector(Start_Cluster);
						// Reset offset
						sector_offset = 0;
					}

					// Fill buffer1
					Read_Sector(sector+sector_offset, 512, buffer1_g);
					sector_offset++;
					
					LEDS_Off(LED1_port,LED1_pin);
				}
				else
				{
					if(index2>511)
					{
						buf_flag2=0;
						buf_flag1=1;
					}
				}
			}
		}while(buf_flag2==1);
	}while(Start_Cluster != 0x0FFFFFFF); // Look for end of file (0x0FFFFFFF) to stop
}
