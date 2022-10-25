#include "TWI.h"
#include "board_struct.h"


uint8_t TWI_Master_Init(volatile TWI_t * TWI_addr, uint32_t I2C_freq){
	uint16_t prescale = (((F_CPU/F_DIV)/I2C_freq)-16UL)/(2UL*255);
	if(prescale < 1){
		TWI_addr->TWSR = 0b00;
		prescale = 1;
	}
	else if(prescale < 4){
		TWI_addr->TWSR = 0b01;
		prescale = 4;
	}
	else if (prescale < 16){
		TWI_addr->TWSR = 0b10;
		prescale = 16;
	}
	else if (prescale < 64){
		TWI_addr->TWSR = 0b11;
		prescale = 64;
	}
	else{
		return 0xFF;
	}
	
	TWI_addr->TWBR = (((F_CPU/F_DIV)/I2C_freq)-16UL)/(2UL*prescale);
}

uint8_t TWI_Master_Receive(volatile TWI_t * TWI_addr, uint8_t device_addr, uint32_t
int_addr, uint8_t int_addr_sz, uint16_t num_bytes, uint8_t * array_name){
	uint8_t return_value = 0;
	uint8_t index = 0;
	uint8_t status;
	uint8_t temp8;
	
	uint8_t send_value=(device_addr<<1)|0x01;
	
	// Next start a TWI communication (same as transmit)
	TWI_addr->TWCR = ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN));
	
	// Wait for the TWINT bit to be set in the TWCR (same as transmit)
	do
	{
		status = TWI_addr->TWCR;
	}while((status&0x80)==0);
	
	//Read the status value to determine what happens next (same as transmit)
	temp8=((TWI_addr->TWSR)&0xF8);  // Clear lower three bits
	// If start was sent, then send SLA+R (temp==0x10 is for repeated start)
	if((temp8==0x08)||(temp8==0x10))   // Start sent
	{
		TWI_addr->TWDR = send_value;
		TWI_addr->TWCR = ((1<<TWINT)|(1<<TWEN=1));
	}
	
	if(return_value==no_errors){
		status = TWI_addr->TWCR;
		if(temp8==0x40){   // SLA+R sent, ACK received
			if(num_bytes==1)
			{
				TWI_addr->TWCR = ((1<<TWINT)|(0<<TWEA)|(1<<TWEN));
			}
			else
			{
				TWI_addr->TWCR = ((1<<TWINT)|(1<<TWEA)|(1<<TWEN));
			}
			
			// Use a while loop to send data bytes until all bytes are sent or
			// an error occurs
			index=0;
			while((num_bytes!=0)&&(return_value==no_errors))
			{
				// Wait until TWINT is set
				do
				{
					status = TWI_addr->TWCR;
				}while((status&0x80)==0);
				// Read the status value to determine what to do next
				temp8=((TWI_addr->TWSR)&0xF8);  // Clear lower three bits
			}
			if(temp8==0x50)   // Data byte received, ACK sent
			{
				num_bytes--;
				array_name[index]=TWI_addr->TWDR;
				index++;
				if(num_bytes==1)
				{
					TWI_addr->TWCR=((1<<TWINT)|(0<<TWEA)|(1<<TWEN));
				}
				else
				{
					TWI_addr->TWCR=((1<<TWINT)|(1<<TWEA)|(1<<TWEN));
				}
			}
			else if(temp8==0x58)  // Data byte received, NACK sent
			{
				// save byte to array and num_bytes--
				TWI_addr->TWCR=((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));
				// Wait for TWSTO to return to �0�
		}
		
		else{ //NACK
			return 0xFF;
			}
			//else NACK
		
	}
	return return_value;
}