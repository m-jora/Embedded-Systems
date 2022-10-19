#ifndef BOARD_H_
#define BOARD_H_

#include <avr/io.h>
#include <stdint.h>

#define F_CPU (16000000UL)
#define F_DIV (1)

#define LED_ON 0
#define LED_OFF 1

/* Registers and associated bit numbers */

#define PIN_A    _SFR_IO8(0x00)
#define PIN_A7   7
#define PIN_A6   6
#define PIN_A5   5
#define PIN_A4   4
#define PIN_A3   3
#define PIN_A2   2
#define PIN_A1   1
#define PIN_A0  0

#define DDR_A    _SFR_IO8(0x01)
#define DDR_A7   7
#define DDR_A6   6
#define DDR_A5   5
#define DDR_A4   4
#define DDR_A3   3
#define DDR_A2   2
#define DDR_A1   1
#define DDR_A0   0

#define PORT_A   _SFR_IO8(0x02)
#define PORT_A7  7
#define PORT_A6  6
#define PORT_A5  5
#define PORT_A4  4
#define PORT_A3  3
#define PORT_A2  2
#define PORT_A1  1
#define PORT_A0  0

#define PIN_B    _SFR_IO8(0x03)
#define PIN_SCK   7
#define PIN_MISO  6
#define PIN_MOSI  5
#define PIN_SDCS 4
#define PIN_B3   3
#define PIN_B2   2
#define PIN_B1   1
#define PIN_B0   0

#define DDR_B    _SFR_IO8(0x04)
#define DDR_SCK   7
#define DDR_MISO  6
#define DDR_MOSI  5
#define DDR_SDCS 4
#define DDR_B3   3
#define DDR_B2   2
#define DDR_B1   1
#define DDR_B0   0

#define PORT_B   _SFR_IO8(0x05)
#define PORT_SCK   7
#define PORT_MISO  6
#define PORT_MOSI  5
#define PORT_SDCS 4
#define PORT_B3  3
#define PORT_B2  2
#define PORT_B1  1
#define PORT_B0  0


#define PIN_C    _SFR_IO8(0x06)
#define PIN_USER_LED   7
#define PIN_C6   6
#define PINTDI   5
#define PINTDO   4
#define PINTMS   3
#define PINTCK   2
#define PIN_SDA   1
#define PIN_SCL   0

#define DDR_C    _SFR_IO8(0x07)
#define DDR_USERLED   7
#define DDR_C6   6
#define DDRTDI   5
#define DDRTDO   4
#define DDRTMS   3
#define DDRTCK   2
#define DDR_SDA   1
#define DDR_SCL   0

#define PORT_C   _SFR_IO8(0x08)
#define PORT_USERLED  7
#define PORT_C6  6
#define PORTTDI  5
#define PORTTDO  4
#define PORTTMS  3
#define PORTTCK  2
#define PORT_SDA  1
#define PORT_SCL  0

#define PIN_D    _SFR_IO8(0x09)
#define PIN_D7   7
#define PIN_D6   6
#define PIN_D5   5
#define PIN_D4   4
#define PIN_D3   3
#define PIN_D2   2
#define PIN_D1   1
#define PIN_D0   0

#define DDR_D    _SFR_IO8(0x0A)
#define DDR_D7   7
#define DDR_D6   6
#define DDR_D5   5
#define DDR_D4   4
#define DDR_D3   3
#define DDR_D2   2
#define DDR_D1   1
#define DDR_D0   0

#define PORT_D   _SFR_IO8(0x0B)
#define PORT_D7  7
#define PORT_D6  6
#define PORT_D5  5
#define PORT_D4  4
#define PORT_D3  3
#define PORT_D2  2
#define PORT_D1  1
#define PORT_D0  0

#define SPI0_base (0x4C)
#define SPI1_base (0xAC)


/***** Microcontroller SFR Addresses *******/
/***** UARTS ******/
typedef struct UART
{
	volatile uint8_t UCSRA;
	volatile uint8_t UCSRB;
	volatile uint8_t UCSRC;
	volatile uint8_t UCSRD;
	volatile uint8_t UBBRL;
	volatile uint8_t UBBRH;
	volatile uint8_t UDR;
} UART_t;

#define UART0 ((volatile UART_t *) 0xC0)
#define UART1 ((volatile UART_t *) 0xC8)
#define UART2 ((volatile UART_t *) 0xD0)

/***** Ports ******/
typedef struct GPIO
{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;

#define PA ((volatile GPIO_t *) 0x20)
#define PB ((volatile GPIO_t *) 0x23)
#define PC ((volatile GPIO_t *) 0x26)
#define PD ((volatile GPIO_t *) 0x29)
#define PE ((volatile GPIO_t *) 0x2C)


#endif /* BOARD_H_ */