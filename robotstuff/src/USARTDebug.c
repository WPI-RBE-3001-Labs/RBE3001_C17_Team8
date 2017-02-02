#include "RBELib/USARTDebug.h"
#include "RBELib/RBELib.h"


void debugUSARTInit(unsigned long baudrate){
  //Calculation given in table 16-1 on page 174
  //Sets the baudrate to whatever is desired
  UBRR1 = (18432000 / (16 * baudrate)) - 1;

  //Control and status register B
  //Bits 7 - 5: Diables interrupts
  //Bit 4: enable Rx
  //Bit 3: enable Tx
  //Bit 2: Part of setting char size to 8 as per table 16-7 on 192
  //Bits 1 - 0: Not used with 8 bit mode
  UCSR1B = 0x18;

  //Control and status register C
  //Bits 7 - 6: Async mode
  //Bits 5 - 4: No parity
  //Bit 3: 1 stop bit
  //Bits 2 - 1: 8 data bits
  //Bit 0: Needs to be set to 0 when in async
  UCSR1C = 0x06;
}


void putCharDebug(char byteToSend){
  //Wait on UDRE1 bit
  //while(!(UCSR1A & 0x20));
  while(!(UCSR1A&(1<<UDRE1)));
  //Fill the buffer
  UDR1 = byteToSend;
}

unsigned char getCharDebug(void) {
    // Wait for incoming data
    while ( !(UCSR1A & (_BV(RXC1))) );
    // Return the data
    return UDR1;
}


//void debugUSARTInit(unsigned long baudrate)
//{
//	UBRR1H = (F_CPU / (16 * baudrate) - 1) >> 8;//set that BAUD rate
//	UBRR1L = (F_CPU / (16 * baudrate) - 1);
//
//	UCSR1B = (1 << RXEN1) | (1 << TXEN1);//enable reciever and transmitter
//
//	UCSR1C = (1 << USBS1) | (3 << UCSZ10); // set data formnat,, 8 data 1 stop
//}
//
//
//
//
//
//void putCharDebug(char byteToSend)
//{
//	while (!( UCSR1A & (1 << UDRE1)));//wait for transfer buffer to empty
//
//	UDR1 = byteToSend;//put the data into the buffer to send
//}
//
//
//
//
//unsigned char getCharDebug(void)
//{
//	while (!(UCSR1A & (1 << RXC1))); //wait instead for data to be received
//
//
//	return UDR1; // return that which is in the buffer
//}




