#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define CPOL_bit (0)
#define CPHA_bit (0)
#define SPCR (0)
#define SPSR (1)
#define SPDR (2)
#define SD_SPI_port SPI0_base
#define clock_rate_error (0xFF)
#define CMD0 (0x40)
#define CMD8 (0x48)
#define CMD58 (0x7A)
#define CMD55 (0x77) // (to_hex)(55+64) = CMD address
#define CMD41 (0x69) // Nice
#define no_errors (0x00)


uint8_t SPI_Master_Init(uint8_t volatile *SPI_addr, uint32_t clock_rate);
uint8_t SPI_Transfer(uint8_t volatile *SPI_addr, uint8_t send_value);

#endif
