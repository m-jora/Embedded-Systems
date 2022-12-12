#ifndef _STA013_Config_H
#define _STA013_Config_H

#include "board_struct.h"



#define STA013 (0x43)
/******* STA013 IO Definitions ********/
#define STA013_TWI_Port (TWI1)  // Defines which TWI port is used for STA013 communication
#define STA013_SPI_Port (SPI0)  // Defines which SPI port is used for STA013 communication
#define STA013_BITEN_Port (PD)  // Defines the GPIO Port for STA013 BITEN
#define STA013_BITEN_Pin (1<<6) // Defines the GPIO Pin for STA013 BITEN
#define STA013_Reset_Port (PB)  // Defines the GPIO Port for STA013 Reset
#define STA013_Reset_Pin (1<<1) // Defines the GPIO Pin for STA013 Reset
#define STA013_DATAREQ_Port (PC)  // Defines the GPIO Port for STA013 DATAREQ
#define STA013_DATAREQ_Pin (1<<6) // Defines the GPIO Pin for STA013 DATAREQ


// ------ Public function prototypes -------------------------------
void STA013_Reset_Init(void);

void STA013_Reset_Control(uint8_t control);

void STA013_BIT_EN_Init(void);

void STA013_BIT_EN_Control(uint8_t control);

void STA013_DATA_REQ_Init(void);

uint8_t STA013_Init(void);



#endif