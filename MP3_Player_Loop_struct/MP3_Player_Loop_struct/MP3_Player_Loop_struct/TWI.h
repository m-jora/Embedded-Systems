#ifndef _TWI_H
#define _TWI_H

#include "board_struct.h"




#define no_errors (0)
#define bus_busy_error (0x81)
#define NACK_error (0x82)
#define TWI_Clock_error (0x87)

// ------ Public function prototypes -------------------------------
uint8_t TWI_Master_Init(volatile TWI_t *TWI_addr, uint32_t I2C_FREQ);
uint8_t TWI_Master_Transmit(volatile TWI_t *TWI_addr, uint8_t device_addr, uint32_t internal_addr, uint8_t int_addr_bytes, uint8_t num_bytes, uint8_t *send_array);
uint8_t TWI_Master_Receive(volatile TWI_t *TWI_addr, uint8_t device_addr, uint32_t internal_addr, uint8_t int_addr_bytes, uint8_t num_bytes, uint8_t *rec_array);





#endif