#ifndef _Long_Serial_In_H
#define _Long_Serial_In_H


// ASCII characters
#define space (0x20)
#define CR (0x0D)
#define LF (0x0A)
#define BS (0x08)
#define DEL (0x7F)



// ------ Public function prototypes -------------------------------

uint32_t Long_Serial_Input(volatile UART_t * UART_addr);


#endif