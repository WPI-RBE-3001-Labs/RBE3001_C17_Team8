#include "RBELib/RBELib.h"
#include "RBELib/SPI.h"


void initSPI(){

//	OLD CODE!!!
//
//	DAC_SS = HIGH; //Select DAC as off
//	PORTCbits._P4 = HIGH;
//	PORTCbits._P5 = HIGH;
//	PORTDbits._P7 = HIGH;
//
//	SPI_MOSI_DDR = OUTPUT; //set MOSI as output
//	SPI_MISO_DDR = INPUT; //set MISO line as input
//	SPI_SCK_DDR  = OUTPUT; //start transmitting output of clock on P7
//	SPI_MASTER_SS = OUTPUT; //SET B P4 TO GO
//	DAC_SS_ddr = OUTPUT;  //Slave select of that DAC bb, output mode
//
//
//	PRR = 0; //SPI must be on
//
//	PORTBbits._P4 = HIGH;//Slave Select the ATmega as HIGH
//
//	//here we have the wild MOSI, SCK and SS as output.
//	//DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4)
//
//	SPCR = //SPI control register 0, apparently SPCR0 in datasheet, SPCR in iomxx4.h
//	(0<< SPIE	)| //Bit causes SPI interrupt to be executed
//	(1<< SPE	)| //SPI enable, must be set to one for enabling.
//	(0<< DORD	)| //1, LSB of data sent dfirst. 0,MSB sent first
//	(1<< MSTR	)| //master/slave select (master on 1, slave on 0)
//	(0<< CPOL	)| //clock polarity
//	(0<< CPHA	)| //clock phase
//	(0<< SPR1	)| //still the prescaler
//	(1<< SPR0	); //its the prescaler
//
//
//	SPSR =
//	(0<< SPIF	)| //interrupt flag
//	(0<< WCOL	)| //write collision flag
//	(0<< SPI2X	); //double speed SPI
//}
//
// BEGIN OF NEW REFACTORED CODE


	SPI_MOSI_DDR = OUTPUT; 	//Set MOSI as output
	SPI_MISO_DDR = INPUT; 	//Set MISO line as input
	SPI_SCK_DDR  = OUTPUT; 	//Start transmitting output of clock on P7
	SPI_MASTER_SS = HIGH; 	//SET B P4 TO GO
	DAC_SS_ddr = OUTPUT;  	//Slave select of that DAC bb, output mode

	PRR = 0; //SPI must be on

	AUX_DAC_SS_ddr = OUTPUT; 	//SPI Slave Select Unused, DDRCbits._P6
	ENCODER_SS_0_ddr = OUTPUT;	//SPI Slave Select Encoder 0, DDRCbits._P5
	ENCODER_SS_1_ddr = OUTPUT;	//SPI Slave Select Encoder 1, DDRCbits._P4
	ENCODER_SS_2_ddr = OUTPUT;	//SPI Slave Select Encoder 2, DDRCbits._P3
	ENCODER_SS_3_ddr = OUTPUT;	//SPI Slave Select Encoder 3, DDRCbits._P2
	SPARE_SS_ddr = OUTPUT;		//SPI Slave Select Unused, DDRCbits._P0


	// Pull all slave selects low
	DAC_SS = HIGH;			//SPI Slave Select DAC 1, PORTDbits._P4
	AUX_DAC_SS = HIGH;		//SPI Slave Select DAC 2, PORTCbits._P6
	ENCODER_SS_0 = HIGH;	//SPI Slave Select Encoder 0, PORTCbits._P5
	ENCODER_SS_1 = HIGH;	//SPI Slave Select Encoder 1, PORTCbits._P4
	ENCODER_SS_2 = HIGH;	//SPI Slave Select Encoder 2, PORTCbits._P3
	ENCODER_SS_3 = HIGH;	//SPI Slave Select Encoder 3, PORTCbits._P2
	PORTCbits._P0 = HIGH;	//SPI Slave Select Unused

	DDRDbits._P7 = OUTPUT; //SPI Accel configure D7
	PORTDbits._P7 = HIGH; // SPI Accel configure D7


	//here we have the wild MOSI, SCK and SS as output.
	//DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4)

	SPCR = 		   //SPI control register 0, apparently SPCR0 in datasheet, SPCR in iomxx4.h
	(0<< SPIE	)| //Bit causes SPI interrupt to be executed
	(1<< SPE	)| //SPI enable, must be set to one for enabling.
	(0<< DORD	)| //1, LSB of data sent dfirst. 0,MSB sent first
	(1<< MSTR	)| //master/slave select (master on 1, slave on 0)
	(0<< CPOL	)| //clock polarity
	(0<< CPHA	)| //clock phase
	(1<< SPR1	)| //still the prescaler
	(1<< SPR0	); //its the prescaler


	SPSR =
	(0<< SPIF	)| //interrupt flag
	(0<< WCOL	)| //write collision flag
	(0<< SPI2X	); //double speed SPI
}

unsigned char spiTransceive(BYTE data){

	//read data, still why is it SPDR, it's SPDR0 on the datasheet
	SPDR = data; //write data

	while(!(SPSR & (1<<SPIF))); //wait for transmission
	return SPDR; //return the received data

}

