#ifndef SD_H_
#define SD_H_

#define SD_CS_port (PB) //Was PIN_B
#define SD_CS_pin (1<<4)

void SD_CS_active (volatile uint8_t *port, uint8_t pin);
void SD_CS_inactive (volatile uint8_t *port, uint8_t pin);
uint8_t SD_Card_Init (void);
uint8_t Read_Block (uint16_t number_of_bytes, uint8_t * array);
#endif