#include <stdio.h>
#include "board_struct.h"
#include "UART.h"
#include "UART_Print.h"
#include "File_System_struct.h"
#include "print_memory.h"
#include "Directory_Functions_struct.h"
#include "Read_Sector.h"
#include <avr/pgmspace.h>
#include "Play_Song.h"


/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A)







// Read Value Functions

/***********************************************************************
DESC: Reads a specified byte from a block of memory 
INPUT: Offset to the byte, Starting address of the block of memory
RETURNS: uint8_t specifed byte
CAUTION: 
************************************************************************/
uint8_t read8(uint16_t offset, uint8_t * array_name)
{
   uint8_t return_val;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=*(array_name+offset);
   return return_val;
}

/***********************************************************************
DESC: Reads a specified integer (stored little endian) from a block of memory 
INPUT: Offset to the LSB of the integer, Starting address of the block of memory
RETURNS: uint16_t specfied integer
CAUTION: 
************************************************************************/

uint16_t read16(uint16_t offset, uint8_t * array_name)
{
   uint16_t return_val;
   uint8_t temp8;
   return_val=0;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=*(array_name+offset+1);
   return_val=(return_val<<8);
   temp8=*(array_name+offset);
   return_val|=temp8;
   return return_val;
}

/***********************************************************************
DESC: Reads a specified word (stored little endian) from a block of memory 
INPUT: Offset to the LSB of the word, Starting address of the block of memory
RETURNS: uint32_t specfied word
CAUTION: 
************************************************************************/

uint32_t read32(uint16_t offset, uint8_t * array_name)
{
   uint32_t return_val;
   uint8_t temp8;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=*(array_name+offset+3);
   return_val=(return_val<<8);
   temp8=*(array_name+offset+2);
   return_val|=temp8;
   return_val=(return_val<<8);
   temp8=*(array_name+offset+1);
   return_val|=temp8;
   return_val=(return_val<<8);
   temp8=*(array_name+offset);
   return_val|=temp8;
   return return_val;
}

//**** Strings for printed output stored in Flash Memory ******
const char Mnt_Drv_1[26] PROGMEM = {"Locating Boot Sector...\n\r"};
const char Mnt_Drv_2[20] PROGMEM = {"Relative Sectors = "};
const char Mnt_Drv_3[14] PROGMEM = {"Disk Error!\n\r"};
const char Mnt_Drv_4[20] PROGMEM = {"FAT16 Detected...\n\r"};
const char Mnt_Drv_5[20] PROGMEM = {"FAT32 Detected...\n\r"};
const char Mnt_Drv_6[14] PROGMEM = {"StartofFAT = "};
const char Mnt_Drv_7[16] PROGMEM = {"FirstDataSec = "};
const char Mnt_Drv_8[20] PROGMEM = {"Relative Sectors = "};

//*************************************************************/

/***********************************************************************
DESC: Determines FAT type, Start of the FAT, Root Directory Sector
      and Start of Data for an SD card
INPUT: Start of a block of memory in RAM that can be used 
       to read blocks from the SD card
RETURNS: uint8_t error code    Creates globals in a struct for: 
uint32_t FATSz, RelSec, RootClus, FirstDataSec, StartofFAT;
uint16_t BytesPerSec, BytesPerSecM, RootEntCnt;
uint8_t SecPerClus, FATtype, SecShift;

CAUTION: 
************************************************************************/


uint8_t Mount_Drive(uint8_t * array_in)
{
  uint32_t TotSec, DataSec, CntClus,MBR_RelSec, FATSz, RootClus;
  uint16_t RsvdSecCnt, RootEntCnt, temp16;
  uint8_t NumFAT, temp8;
  uint8_t *values;
  char *prnt_bffr;
  FS_values_t * Drive_p;

  Drive_p=Export_Drive_values();
  MBR_RelSec=0;
  RootClus=0;
  values=array_in;
  prnt_bffr=Export_print_buffer();
  
  Copy_String_to_Buffer(Mnt_Drv_1,0,prnt_bffr);
  UART_Transmit_String(UART1,0,prnt_bffr);
  //Locating Boot Sector...
  Read_Sector(0,512,values);
  temp8=read8(0,values);
  if((temp8!=0xEB)&&(temp8!=0xE9))
  {
	  MBR_RelSec=read32(MBR_RelSectors,values);
	  Read_Sector(MBR_RelSec,512,values);
	  Copy_String_to_Buffer(Mnt_Drv_2,0,prnt_bffr);
	  UART_Transmit_String(UART1,0,prnt_bffr);
	  sprintf(prnt_bffr,"%lu\n\r",MBR_RelSec);
	  UART_Transmit_String(UART1,0,prnt_bffr);
	  temp8=read8(0,values);
  }
  if((temp8!=0xEB)&&(temp8!=0xE9))
  {
	  //Disk Error!
	  Copy_String_to_Buffer(Mnt_Drv_3,0,prnt_bffr);
	  UART_Transmit_String(UART1,0,prnt_bffr);
	  return Disk_Error;
  }
  Drive_p->BytesPerSec=read16(BPB_BytesPerSector,values);
  Drive_p->BytesPerSecShift=0;
  temp16=Drive_p->BytesPerSec;
  while(temp16!=0x01)
  {
	  Drive_p->BytesPerSecShift++;
	  temp16=temp16>>1;
  }
  Drive_p->SecPerClus=read8(BPB_SectorsPerCluster,values);
  RsvdSecCnt=read16(BPB_RsvdSectorCount,values);
  NumFAT=read8(BPB_NumberFATs,values);
  RootEntCnt=read16(BPB_RootEntryCount,values);
  TotSec=read16(BPB_TotalSectors16,values);
  if(TotSec==0) TotSec=read32(BPB_TotalSectors32,values);
  FATSz=read16(BPB_FATsize16,values);
  if(FATSz==0)
  {
	  FATSz=read32(BPB_FATsize32,values);
  }
  Drive_p->RootDirSecs=((RootEntCnt * 32) + (Drive_p->BytesPerSec - 1))/Drive_p->BytesPerSec;
  DataSec=TotSec-(RsvdSecCnt+(NumFAT*FATSz)+Drive_p->RootDirSecs);
  CntClus=DataSec/Drive_p->SecPerClus;
  Drive_p->StartofFAT=RsvdSecCnt+MBR_RelSec;
  Copy_String_to_Buffer(Mnt_Drv_6,0,prnt_bffr);
  UART_Transmit_String(UART1,0,prnt_bffr);
  sprintf(prnt_bffr,"%lu\n\r",Drive_p->StartofFAT);
  UART_Transmit_String(UART1,0,prnt_bffr);
  Drive_p->FirstDataSec=Drive_p->StartofFAT+(NumFAT*FATSz)+Drive_p->RootDirSecs;
  Copy_String_to_Buffer(Mnt_Drv_7,0,prnt_bffr);
  UART_Transmit_String(UART1,0,prnt_bffr);
  sprintf(prnt_bffr,"%lu\n\r",Drive_p->FirstDataSec);
  UART_Transmit_String(UART1,0,prnt_bffr);
  if(CntClus<65525)
  {
	  //FAT16 Detected...
	  Drive_p->FATtype=FAT16;
	  Drive_p->FATshift=FAT16_shift;
	  Copy_String_to_Buffer(Mnt_Drv_4,0,prnt_bffr);
	  UART_Transmit_String(UART1,0,prnt_bffr);
	  Drive_p->FirstRootDirSec=Drive_p->StartofFAT+(NumFAT*FATSz);
  }
  else
  {
	  //FAT32 Detected...
	  Drive_p->FATtype=FAT32;
	  Drive_p->FATshift=FAT32_shift;
	  RootClus=read32(BPB_RootCluster,values);
	  Copy_String_to_Buffer(Mnt_Drv_5,0,prnt_bffr);
	  UART_Transmit_String(UART1,0,prnt_bffr);
	  Drive_p->FirstRootDirSec=((RootClus-2)*Drive_p->SecPerClus)+Drive_p->FirstDataSec;
  }
  Copy_String_to_Buffer(Mnt_Drv_8,0,prnt_bffr);
  UART_Transmit_String(UART1,0,prnt_bffr);
  sprintf(prnt_bffr,"%lu\n\r",Drive_p->FirstRootDirSec);
  UART_Transmit_String(UART1,0,prnt_bffr);
  return No_Disk_Error;
}


/***********************************************************************
DESC: Calculates the First Sector of a given Cluster 
INPUT: uint32_t Cluster number
RETURNS: uint32_t sector number 
CAUTION: 
************************************************************************/
uint32_t First_Sector (uint32_t Cluster_num)
{
   uint32_t Sector_num;
   FS_values_t * Drive_p;

   Drive_p=Export_Drive_values();
   if(Cluster_num==0)
   {
	   Sector_num=Drive_p->FirstRootDirSec;
   }
   else
   {
	   Sector_num=((Cluster_num-2)*Drive_p->SecPerClus)+Drive_p->FirstDataSec;
   }
   return Sector_num;
}





/***********************************************************************
DESC: Finds the next cluster number of a file in the FAT with an input
      parameter of the current cluster number.
INPUT: Current cluster number.
RETURNS: Next Cluster number from the FAT
CAUTION: 
************************************************************************/


uint32_t Find_Next_Clus(uint32_t Cluster_num, uint8_t * array_name)
{
   uint32_t Sector, return_clus=0;
   uint16_t FAToffset;
   uint8_t * values;
   FS_values_t * Drive_p;

   Drive_p=Export_Drive_values();
   values=array_name;
   Sector=(Cluster_num>>(Drive_p->BytesPerSecShift-Drive_p->FATshift))+Drive_p->StartofFAT;
   Read_Sector(Sector, Drive_p->BytesPerSec,values);
   FAToffset=(uint16_t)((Cluster_num<<Drive_p->FATshift)&(Drive_p->BytesPerSec-1));
   if(Drive_p->FATtype==FAT16)    // FAT16
   {
	   return_clus=(uint32_t)(read16(FAToffset,values));
	   if(return_clus==0x0000FFFF) return_clus=0x0FFFFFFF; // EOF for FAT16 changed to EOF for FAT32
   }
   else //if(Drive_p->FATtype==FAT32)    // FAT32
   {
	   return_clus=(read32(FAToffset,values)&0x0FFFFFFF);
   }
   return return_clus;
}
   



/***********************************************************************
DESC: Prints the sectors of a file until the user hits X
      Prints the Cluster Number and Sector number of each sector as well
INPUT: Starting Cluster of the file and the start of a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint8_t 0
CAUTION: 
************************************************************************/


uint8_t Open_File(uint32_t Cluster_num, uint8_t * array_in)
{
 uint32_t Sector=0, SecOffset, Start_Cluster;
 uint8_t temp8;
 uint8_t * values;
 char *prnt_bffr;
 FS_values_t * Drive_p;

 Drive_p=Export_Drive_values();
 prnt_bffr=Export_print_buffer();
 values=array_in;
 SecOffset=0;
 Start_Cluster=Cluster_num;
 
 do
 {
	 if(SecOffset==0) Sector=((Cluster_num-2)*Drive_p->SecPerClus)+Drive_p->FirstDataSec;
	 sprintf(prnt_bffr,"Cluster#: %9lu,  Sector#: %9lu,  SecOffset: %lu\n\r",Cluster_num,(Sector+SecOffset),SecOffset);
	 UART_Transmit_String(UART1,0,prnt_bffr);
	 Read_Sector((Sector+SecOffset), Drive_p->BytesPerSec, values);
	 print_memory(UART1,512,values);
	 SecOffset++;
	 if(SecOffset==Drive_p->SecPerClus)
	 {
		 Cluster_num=Find_Next_Clus(Cluster_num,values);
		 SecOffset=0;
	 }
	 sprintf(prnt_bffr,"Press Space to Continue or X to exit\n\r");
	 UART_Transmit_String(UART1,0,prnt_bffr);
	 do
	 {
		 temp8=UART_Receive(UART1);
	 }while((temp8!=0x20)&&(temp8!='X')&&(temp8!='x')&&(temp8!='P'));
 }while(temp8==0x20);
 if(temp8=='P')
 {
	 Play_Song(Start_Cluster);
 }
 UART_Transmit(UART1,CR);
 UART_Transmit(UART1,LF);
 UART_Transmit(UART1,LF);
 return 0;
}




