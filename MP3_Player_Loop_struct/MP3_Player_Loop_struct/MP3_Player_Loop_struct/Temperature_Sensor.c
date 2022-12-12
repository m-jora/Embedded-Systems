/*
 * Temperature_Sensor.c
 *
 * Created: 7/2/2020 2:54:03 PM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "TWI.h"

#define Temp_Sensor (0x4F)

void Temperature_Sensor_Init(void)
{
	uint8_t write_array[3];
	
	//write_array[0]=0x01;
	write_array[0]=0x64;
	
	TWI_Master_Transmit(TWI1,Temp_Sensor,1,1,1,write_array);
	
	//write_array[0]=0x02;  // Low Temp Limit pointer
	write_array[0]=0x1A;  // about 80F
	write_array[1]=0xB0;  // fraction
	
	TWI_Master_Transmit(TWI1,Temp_Sensor,2,1,2,write_array);
	
	//write_array[0]=0x03;  // High Temp Limit pointer
	write_array[0]=0x1B;  // about 82F
	write_array[1]=0xD0;  // fraction
	
	TWI_Master_Transmit(TWI1,Temp_Sensor,3,1,2,write_array);
}

uint16_t Temperature_Read(void)
{
	uint8_t comm_array[3];
	uint16_t return_val;
	comm_array[0]=0x00;
	
	TWI_Master_Receive(TWI1,Temp_Sensor,0,1,2,comm_array);
	
	return return_val=(comm_array[0]*256)|comm_array[1];
}