#ifndef TWI_H_
#define TWI_H_

/* Checklist:
1.	D
2.	D
3.	
4.	
5.	
6.	
7.	
8.	
*/

uint8_t TWI_Master_Init(volatile TWI_t * TWI_addr, uint32_t I2C_freq);
uint8_t TWI_Master_Receive(volatile TWI_t * TWI_addr, uint8_t device_addr, uint32_t
int_addr, uint8_t int_addr_sz, uint16_t num_bytes, uint8_t * array_name);

#endif