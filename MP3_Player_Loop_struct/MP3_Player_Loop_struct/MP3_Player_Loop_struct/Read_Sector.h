#ifndef _Read_Sector_H
#define _Read_Sector_H


//------- Public Constants  -----------------------------------------
#define Disk_Error (0xF0)
#define No_Disk_Error (0)

// ------ Public function prototypes -------------------------------


uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data);



#endif