#ifndef _SPI_H
#define _SPI_H

#include "board_struct.h"

//------- Public Constant definitions --------------------------------
// error values
#define init_okay (0)
#define no_errors (0)
#define SPI_Write_Collision (0xC0)
#define illegal_clockrate (0x0F)
#define illegal_port (0x0E)
#define SPI_TIMEOUT (0x80)


// ------ Public function prototypes -------------------------------
uint8_t SPI_Master_Init(volatile SPI_t *SPI_addr, uint32_t clock_rate);
uint8_t SPI_Transmit(volatile SPI_t *SPI_addr, uint8_t data_input);
uint8_t SPI_Receive(volatile SPI_t *SPI_addr, uint8_t * data_output);
uint8_t SPI_Transfer(volatile SPI_t *SPI_addr, uint8_t data_input, uint8_t * data_output);

#endif