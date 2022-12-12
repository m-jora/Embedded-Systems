#include "board_struct.h"
#include "SDCard.h"
#include "Read_Sector.h"




uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data)
{
	uint8_t SDtype,error_flag=No_Disk_Error;

	SDtype=Return_SD_Card_Type();
	SD_Card_CS_Active(SD_CS_Port,SD_CS_Pin);  // Clear nCS = 0
	error_flag=Send_Command(CMD17,(sector_number<<SDtype));
	if(error_flag==no_errors) error_flag=Read_Block(sector_size,array_for_data);
	SD_Card_CS_Inactive(SD_CS_Port,SD_CS_Pin);  // Set nCS = 1

	if(error_flag!=no_errors)
	{
		error_flag=Disk_Error;
	}
	
    return error_flag;
}
