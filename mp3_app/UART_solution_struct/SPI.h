#include <stdint.h>

#define CPOL_bit (0)
#define CPHA_bit (0)
#define SPCR (0)
#define SPSR (1)

#define clock_rate_error (0xFF)


uint8_t SPI_Master_Init(uint8_t volatile *SPI_addr, uint32_t clock_rate);