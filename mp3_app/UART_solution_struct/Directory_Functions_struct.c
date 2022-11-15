#include <stdio.h>
#include "board_struct.h"
#include "UART.h"
//#include "File_System_struct.h"
#include "Directory_Functions_struct.h"
//#include "Read_Sector.h"
#include "UART_Print.h"
#include "SD.h"

/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A) 

FS_values_t Drive_values;

/***********************************************************************
DESC: Returns a pointer to the global structure Drive_values to export to other files
INPUT: void
RETURNS: Pointer to the structure Drive_values
CAUTION: 
************************************************************************/

FS_values_t * Export_Drive_values(void)
{
   return &Drive_values;
}


/***********************************************************************
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/



uint16_t  Print_Directory(uint32_t Sector_num, uint8_t * array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t * values;
   char *prnt_bffr;

   prnt_bffr=Export_print_buffer();
   
   values=array_in;
   entries=0;
   i=0;
   if (Drive_values.FATtype==FAT16)  // included for FAT16 compatibility
   { 
      max_sectors=Drive_values.RootDirSecs;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=Drive_values.SecPerClus;
   }
   Sector=Sector_num;
   error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
   print_memory(UART1, 512, values);
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00)) // -------------------------temp8 = 0x00 for some reason--------
	    {  
	       attr=read8(0x0b+i,values);
		   if((attr&0x0E)==0)   // if hidden, system or Vol_ID bit is set do not print
		   {
		      entries++;
			  sprintf(prnt_bffr,"%5d. ",entries);  // print entry number with a fixed width specifier
		      UART_Transmit_String(UART1,0,prnt_bffr);
			  for(j=0;j<8;j++)
			  {
			     out_val=read8(i+j,values);   // print the 8 byte name
			     UART_Transmit(UART1,out_val);
			  }
              if((attr&0x10)==0x10)  // indicates directory
			  {
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        UART_Transmit(UART1,out_val);
			     }
			     sprintf(prnt_bffr,"[DIR]\r\n");
				 UART_Transmit_String(UART1,0,prnt_bffr);
			  }
			  else       // print a period and the three byte extension for a file
			  {
			     UART_Transmit(UART1,0x2E);       
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        UART_Transmit(UART1,out_val);
			     }
			     UART_Transmit(UART1,CR);
                 UART_Transmit(UART1,LF);
			  }
		    }
		}
		    i=i+32;  // next entry
		    if(i>510)
		    {
			  Sector++;
              if((Sector-Sector_num)<max_sectors)
			  {
                 error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
			     if(error_flag!=no_errors)
			     {
			        entries=0;   // no entries found indicates disk read error
				    temp8=0;     // forces a function exit
			     }
                 i=0;
			  }
			  else
			  {
			     entries=entries|more_entries;  // set msb to indicate more entries in another cluster
			     temp8=0;                       // forces a function exit
			  }
		    }
         
	  }while(temp8!=0);
	}
	else
	{
	   entries=0;    // no entries found indicates disk read error
	}
   return entries;
 }


/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster  
INPUT: Starting Sector of the directory, an entry number and a pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint32_t with cluster in lower 28 bits.  Bit 28 set if this is 
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION: 
************************************************************************/

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t * array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (Drive_values.FATtype==FAT16)  // included for FAT16 compatibility
   { 
      max_sectors=Drive_values.RootDirSecs;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=Drive_values.SecPerClus;
   }
   Sector=Sector_num;
   error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   if((attr&0x0E)==0)    // if hidden do not print
		   {
		      entries++;
              if(entries==Entry)
              {
			    if(Drive_values.FATtype==FAT32)
                {
                   return_clus=read8(21+i,values);
				   return_clus=return_clus<<8;
                   return_clus|=read8(20+i,values);
                   return_clus=return_clus<<8;
                }
                return_clus|=read8(27+i,values);
			    return_clus=return_clus<<8;
                return_clus|=read8(26+i,values);
			    attr=read8(0x0b+i,values);
			    if(attr&0x10) return_clus|=directory_bit;
                temp8=0;    // forces a function exit
              }
              
		    }
		}
		    i=i+32;  // next entry
		    if(i>510)
		    {
			  Sector++;
			  if((Sector-Sector_num)<max_sectors)
			  {
                 error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
			     if(error_flag!=no_errors)
			     {
			         return_clus=no_entry_found;
                     temp8=0; 
			     }
			     i=0;
			  }
			  else
			  {
			     temp8=0;                       // forces a function exit
			  }
		    }
         
	  }while(temp8!=0);
	}
	else
	{
	   return_clus=no_entry_found;
	}
	if(return_clus==0) return_clus=no_entry_found;
   return return_clus;
 }
 
 uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data){
	 uint8_t No_Disk_Error = 0;
	 
	 uint8_t SDtype,error_flag=No_Disk_Error;
	 SDtype = 0;
	 //SDtype=Return_SD_Card_Type(); // HC=0, no change to sector number
	 // SC=9, multiplies sector number by 512 to convert to byte addr.
	 SD_CS_active (SD_CS_port, SD_CS_pin); //nCS0=0;
	 error_flag=Send_Command(17,(sector_number<<SDtype));
	 if(error_flag==no_errors)
	 error_flag=Read_Block(sector_size,array_for_data); //I return error for some reason
	 SD_CS_inactive (SD_CS_port, SD_CS_pin); // nCS0=1;
	 if(error_flag!=no_errors)
	 {
		 error_flag=Disk_Error;
	 }
	 return error_flag;
 }

uint8_t read8 (uint16_t offset, uint8_t * array_name){
	uint8_t temp;
	temp = *(array_name+offset);
	return temp;
}

uint16_t read16 (uint16_t offset, uint8_t * array_name){
	uint16_t return_val;
	uint8_t temp, i;
	return_val=0;
	for(i=0;i<2;i++)
	{
		temp=*(array_name+offset+(1-i)); 
		return_val=return_val<<8;
		return_val|=temp;
	}
	return return_val;
}

uint32_t read32 (uint16_t offset, uint8_t * array_name){
	uint32_t return_val;
	uint8_t temp, i;
	return_val=0;
	for(i=0;i<4;i++)
	{
		temp=*(array_name+offset+(3-i));
		return_val=return_val<<8;
		return_val|=temp;
	}
	return return_val;
}

uint8_t mount_drive(uint8_t * array){
	FS_values_t * Drive_values = Export_Drive_values();
	char * p_buffer;
	p_buffer = Export_print_buffer();
	uint8_t error_flag, NumFATs;
	uint16_t RootEntCnt, TotSec16, RsvdSecCnt;
	uint32_t bpb_addr, TotSec32, DataSec, FATSz32, CountOfClusters, HiddSec, RootClus;
	
	error_flag = Send_Command(17, 0);
	error_flag = Read_Block(512, array);
	
	// Check if MBR
	if((array[0] != 0xEB) && (array[0] != 0xE9)){
		bpb_addr = read32(0x01C6, array);
		
		error_flag = Send_Command(17, bpb_addr);
		error_flag = Read_Block(512, array);
		if((array[0] != 0xEB) && (array[0] != 0xE9)){
			return Disk_Error;
		}
		
		//RootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytsPerSec-1)) / BPB_BytsPerSec
		RootEntCnt = read16(0x0011, array);
		Drive_values->BytesPerSec = read16(0x000B, array);
		Drive_values->RootDirSecs = ((RootEntCnt * 32) + (Drive_values->BytesPerSec - 1)) / Drive_values->BytesPerSec;
		
		//DataSec = TotSec – (BPB_ResvdSecCnt + (BPB_NumFATSs * FATSz) + RootDirSectors)
		TotSec16 = read16(0x13, array);
		TotSec32 = read32(0x20, array);
		RsvdSecCnt = read16(0x000E, array);
		NumFATs = read8(0x0010, array);
		FATSz32 = read32(0x0024, array);
		
		if(FATSz32 == 0){
			return Disk_Error;
		}
		
		if(TotSec16 != 0){
			DataSec = TotSec16 - (RsvdSecCnt + (NumFATs * FATSz32) + Drive_values->RootDirSecs);
		}
		else{
			DataSec = TotSec32 - (RsvdSecCnt + (NumFATs * FATSz32) + Drive_values->RootDirSecs);
		}
		
		// CountOfClusters = DataSec / SecPerClus
		Drive_values->SecPerClus = read8(0x0D, array);
		CountOfClusters = DataSec / Drive_values->SecPerClus;
		
		HiddSec = read32(0x001C, array);
		
		// FirstDataSector = BPB_ResvdSecCnt + (BPB_NumFATs * FATSz) + RootDirSectors + MBR_RelativeSectors
		Drive_values->FirstDataSec = RsvdSecCnt + (NumFATs * FATSz32) + Drive_values->RootDirSecs + HiddSec;
		
		// StartOfFAT = BPB_ResvdSecCnt + MBR_RelativeSectors
		Drive_values->StartofFAT = RsvdSecCnt + HiddSec;
		
		RootClus = read32(0x2C, array);
		
		// FirstRootDirSecNum = ((BPB_RootClus - 2) * BPB_SecPerClus) + FirstDataSector
		Drive_values->FirstRootDirSec = ((RootClus - 2) * Drive_values->SecPerClus) + Drive_values->FirstDataSec;
		
		// Fat Type
		if(CountOfClusters < 65525){
			return Disk_Error;
		}
		else{
			Drive_values->FATtype = 32;
			return no_errors;
		}		
	}
}

uint32_t First_Sector(uint32_t Cluster_Num){
	FS_values_t * Drive_values = Export_Drive_values();
	uint32_t FirstSectorOfCluster;
	if(Cluster_Num == 0){
		return Drive_values->FirstRootDirSec;
	}
	else{
		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		FirstSectorOfCluster = ((Cluster_Num - 2) * Drive_values->SecPerClus) + Drive_values->FirstDataSec;
	}
}

uint32_t Find_Next_Clus(uint32_t cluster_num, uint8_t * array){	FS_values_t * Drive_p = Export_Drive_values();
	uint32_t FATOffset, ThisFATSecNum, ThisFATEntOffset, return_clus;
	
	// Step 1
	uint32_t Sector = ((cluster_num * Drive_p->FATtype) / Drive_p->BytesPerSec) + Drive_p->StartofFAT;
	// Step 2
	Read_Sector(Sector, Drive_p->BytesPerSec,array);
	// Step 3
	ThisFATEntOffset = (FATOffset % Drive_p->BytesPerSec);
	FATOffset = (cluster_num*Drive_p->FATtype);
	ThisFATEntOffset=(uint16_t)(FATOffset % (Drive_p->BytesPerSec));
	// Step 4
	return_clus=(read32(FATOffset, array) & 0x0FFFFFFF);
	
	return return_clus;
}
