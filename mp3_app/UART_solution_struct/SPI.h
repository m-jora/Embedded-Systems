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


uint8_t SPI_Master_Init(uint8_t volatile *SPI_addr, uint32_t clock_rate);
uint8_t SPI_Transfer(uint8_t volatile *SPI_addr, uint8_t send_value);
uint8_t Send_Command (uint8_t command, uint32_t argument);

/*
1.	D
2.	D
3F.	D
4A.	
5A.	
6.	
7A.	
8.	
9.	


*/