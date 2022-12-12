#include <avr/io.h>
#include "board_struct.h"
#include "TWI.h"
#include <stdio.h>

#define START_sent (0x08)
#define r_START_sent (0x10)
#define SLA_W_ACK (0x18)
#define SLA_W_NACK (0x20)
#define DATA_SENT_ACK (0x28)
#define DATA_SENT_NACK (0x30)
#define ARB_LOST (0x38)
#define SLA_R_ACK (0x40)
#define SLA_R_NACK (0x48)
#define DATA_RCV_ACK (0x50)
#define DATA_RCV_NACK (0x58)

/***********************************************************************
DESC:    Sets the Prescale and Clock Rate Generator for the two wire interface
INPUT: Desired SCL clock frequency
RETURNS: Nothing
CAUTION: 
          
************************************************************************/

uint8_t TWI_Master_Init(volatile TWI_t *TWI_addr, uint32_t I2C_FREQ)
{
	uint32_t temp32;
	uint8_t Prescale=0, return_val=0;
	
	temp32 = ((F_CPU/OSC_DIV)/I2C_FREQ)-16;
	while(temp32>256)
	{
		temp32=temp32/4;
		Prescale++;
	}
	if(Prescale>3)
	{
		return_val=TWI_Clock_error;
	}
	else
	{
		(TWI_addr->TWI_TWSR)=Prescale;
		(TWI_addr->TWI_TWBR)=(uint8_t)temp32;
	}
	return return_val;
	
}



/***********************************************************************
DESC:    Creates the signals required for transmitting bytes using
         the I2C bus.
RETURNS: Error Flag
CAUTION: 
          
************************************************************************/

uint8_t TWI_Master_Transmit(volatile TWI_t *TWI_addr, uint8_t device_addr, uint32_t internal_addr, uint8_t int_addr_bytes, uint8_t num_bytes, uint8_t *send_array)
{
  uint8_t send_val, return_val, index, status;
  volatile uint8_t temp8;
  uint16_t timeout; 
  return_val=no_errors;
  send_val=device_addr<<1; // lsb is 0 for W
  index=0;
  timeout=0;
  // Send START condition;  
  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTA=1 for Start, TWEN=1
  do 
  {
	 status=(TWI_addr->TWI_TWCR);
	 timeout++;
  } while (((status&0x80)==0)&&(timeout!=0));
  if(timeout==0)
  {
	  return_val=bus_busy_error;
  }
  else
  {
	  temp8=((TWI_addr->TWI_TWSR)&0xF8); 
	  if((temp8==START_sent)||(temp8==r_START_sent))   // Start sent
	  {
		  (TWI_addr->TWI_TWDR)=send_val;
		  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
	  }
	  else
	  {
		  return_val=bus_busy_error;
	  } 
  }
  
  /**** Send Internal Address *****/
  if((int_addr_bytes!=0)&&(return_val==no_errors))
  {
	  index=4-int_addr_bytes;
	  while((index<4)&&(return_val==no_errors))
	  {
		   send_val=(uint8_t)(internal_addr>>(8*(3-index)));
		   index++;
		   do
		   {
			   status=(TWI_addr->TWI_TWCR);
		   } while ((status&0x80)==0);
		   temp8=((TWI_addr->TWI_TWSR)&0xF8);
		   if(temp8==SLA_W_ACK)   // SLA+W sent, ACK received
		   {
			   (TWI_addr->TWI_TWDR)=send_val;
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   
		   }
		   else if(temp8==DATA_SENT_ACK)  // Data sent, ACK received
		   {
			   (TWI_addr->TWI_TWDR)=send_val;
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   
		   }
		   else if(temp8==SLA_W_NACK)  // SLA+W sent, NACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0);
			   return_val=NACK_error;
		   }
		   else if(temp8==DATA_SENT_NACK)  // Data sent, NACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared
			   return_val=NACK_error;
		   }
		   else if(temp8==ARB_LOST)  // Arbitration lost
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   return_val=bus_busy_error;
		   }
	  }
  }
  index=0;	  
  /**** Send Data Bytes ******/
  if(return_val==no_errors)
  {
	   while((num_bytes!=0)&&(return_val==no_errors))
	   {
		   send_val=send_array[index];
		   index++;
		   num_bytes--;
		   do
		   {
			   status=(TWI_addr->TWI_TWCR);
		   } while ((status&0x80)==0);    // Wait for TWINT to be set
		   temp8=((TWI_addr->TWI_TWSR)&0xF8);
		   if(temp8==SLA_W_ACK)   // SLA+W sent, ACK received
		   {
			   (TWI_addr->TWI_TWDR)=send_val;
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   
		   }
		   else if(temp8==DATA_SENT_ACK)  // Data sent, ACK received
		   {
			   (TWI_addr->TWI_TWDR)=send_val;
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   
		   }
		   else if(temp8==SLA_W_NACK)  // SLA+W sent, NACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared
			   return_val=NACK_error;
		   }
		   else if(temp8==DATA_SENT_NACK)  // Data sent, NACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared
			   return_val=NACK_error;
		   }
		   else if(temp8==ARB_LOST)  // Arbitration lost
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   return_val=bus_busy_error;
		   }
	   }
	   if(return_val==no_errors)
	   {
		   do
		   {
			   status=(TWI_addr->TWI_TWCR);
		   } while ((status&0x80)==0);    // Wait for TWINT to be set
		   temp8=((TWI_addr->TWI_TWSR)&0xF8);
		   if(temp8==SLA_W_ACK)   // SLA+W sent, ACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared // Wait for the Stop flag to be cleared
		   }
		   else if(temp8==DATA_SENT_ACK)  // Data sent, ACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared // Wait for the Stop flag to be cleared
		   }
		   else if(temp8==SLA_W_NACK)  // SLA+W sent, NACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared // Wait for the Stop flag to be cleared
			   return_val=NACK_error;
		   }
		   else if(temp8==DATA_SENT_NACK)  // Data sent, NACK received
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
			   do
			   {
				   status=(TWI_addr->TWI_TWCR);
			   } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared // Wait for the Stop flag to be cleared
			   return_val=NACK_error;
		   }
		   else if(temp8==ARB_LOST)  // Arbitration lost
		   {
			   (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			   return_val=bus_busy_error;
		   }
	   }
  }
	   
  return return_val;
}

/***********************************************************************
DESC:    Creates the signals required for receiving bytes using
         the I2C bus.
RETURNS: Error Flag
CAUTION: 
          
************************************************************************/


uint8_t TWI_Master_Receive(volatile TWI_t *TWI_addr, uint8_t device_addr, uint32_t internal_addr, uint8_t int_addr_bytes, uint8_t num_bytes, uint8_t * rec_array)
{
  uint8_t temp8, send_val, return_val, index, status;
  uint16_t timeout;
  return_val=no_errors;
  if(int_addr_bytes!=0)
  {
	  return_val=TWI_Master_Transmit(TWI_addr,device_addr,internal_addr, int_addr_bytes,0,&send_val);
  }
  send_val=((device_addr<<1)|0x01); // lsb is 1 for R
  index=0;
  timeout=0;
  // Send START
  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTA=1 for Start, TWEN=1
  do
  {
	  status=(TWI_addr->TWI_TWCR);
	  timeout++;
  } while (((status&0x80)==0)&&(timeout!=0)); // Wait for Status flag to be set
  if(timeout==0)
  {
	  return_val=bus_busy_error;
  }
  else
  {
	  temp8=((TWI_addr->TWI_TWSR)&0xF8);
	  if((temp8==START_sent)||(temp8==r_START_sent))   // Start or Repeated Start sent
	  {
		  (TWI_addr->TWI_TWDR)=send_val;
		  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
	  }
	  else
	  {
		  return_val=bus_busy_error;
	  }
  }
  if(return_val==no_errors)
  {
	  do
	  {
		  status=(TWI_addr->TWI_TWCR);
	  } while ((status&0x80)==0); // Wait for Status flag to be set
	  temp8=((TWI_addr->TWI_TWSR)&0xF8);
	  if(temp8==SLA_R_ACK)   // SLA+R sent, ACK received
	  {
		  if(num_bytes==1)
		  {
			 (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEA=0 for NACK, TWEN=1 
		  }
		  else
		  {
			  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEA)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEA=1 for ACK, TWEN=1
		  }
	  }
	  else if(temp8==SLA_R_NACK)  // SLA+R sent, NACK received
	  {
		  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=1 for Stop, TWEN=1
		  do
		  {
			  status=(TWI_addr->TWI_TWCR);
		  } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared // Wait for the Stop flag to be cleared
		  return_val=NACK_error;
	  }
	  else
	  {
		  return_val=bus_busy_error;
	  }
  }
  index=0;
  if(return_val==no_errors)
  {
	  while((num_bytes!=0)&&(return_val==no_errors))
	  {
		  do
		  {
			  status=(TWI_addr->TWI_TWCR);
		  } while ((status&0x80)==0); // Wait for Status flag to be set
		  temp8=((TWI_addr->TWI_TWSR)&0xF8);
		  if(temp8==DATA_RCV_ACK)   // Data byte received, ACK sent
		  {
			num_bytes--;
			rec_array[index]=(TWI_addr->TWI_TWDR);
			index++;
			if(num_bytes==1)
			{
				(TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEA=0 for NACK, TWEN=1
			}
			else
			{
				(TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEA)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEA=1 for ACK, TWEN=1
			}
		  }
		  else if(temp8==DATA_RCV_NACK)  // Data byte received, NACK sent
		  {
			  num_bytes--;
			  rec_array[index]=(TWI_addr->TWI_TWDR);
			  index++;
			  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN));   // TWIINT=1 to clear flag, TWSTO=0 for Stop, TWEN=1
			  do
			  {
				  status=(TWI_addr->TWI_TWCR);
			  } while ((status&(1<<TWSTO))!=0); // Wait for the Stop flag to be cleared // Wait for the Stop flag to be cleared
		  }
		  else if(temp8==ARB_LOST)  // Arbitration lost
		  {
			  (TWI_addr->TWI_TWCR) = ((1<<TWINT)|(1<<TWEN));   // TWIINT=1 to clear flag, TWEN=1
			  return_val=bus_busy_error;
		  }
	  }
  }
  return return_val;
}




