#ifndef SD_H_
#define SD_H_

#define SD_CS_port (PB) //Was PIN_B
#define SD_CS_pin (1<<4)

void SD_CS_active (volatile GPIO_t *port, uint8_t pin);
void SD_CS_inactive (volatile GPIO_t *port, uint8_t pin);
uint8_t SD_Card_Init (void);
uint8_t Read_Block (uint16_t number_of_bytes, uint8_t * array);
uint8_t Send_Command (uint8_t command, uint32_t argument);
uint8_t Receive_Response (uint8_t number_of_bytes, uint8_t * array_name);
#endif