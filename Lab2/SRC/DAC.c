#include "RBELib/RBELib.h"
#include "RBELib/DAC.h"



void setDAC(int DACn, int SPIVal){
	//setPinsVal(); use eventually?


	unsigned char commandAndSelect = 0; //hey look it's something we want to send
	unsigned char dataHigh = 0; //we'll send the big bits
	unsigned char dataLow = 0; //and the lower bits
	//this was a very smart idea, thanks for pointing me in the right direction

	unsigned long temp = 0;//and here's something so that we can arrange things later :^)

	//okay, so if we have selecting through integer, we can make a nice little
	//mask for sending that selection to Mr. DAC

	//0b00110000; so we're looking at commands in the most significant nibble
	//setting the DAC on and stuff, selecting internal reference voltage.

	commandAndSelect = (0x30) | (DACn); //okay that's it.

	//think it's smart to cap it?
	 if(SPIVal >= 4096) SPIVal = 4095;
	//I do

	//now let's do the magic!
	 temp = SPIVal; //put the value into the workplace of temp
	 temp = temp >> 4; //drop the bottom four bits
	 dataHigh = temp; // and now we have our high bits.

	 //so now we check out the third package
	 temp = SPIVal & 0x000F;//we need only the last 4 bits, so mask it and put into temp
	 temp = temp << 4;//shift four, because the LS nibble we don't care about (rip nibble)
	 dataLow = temp;//and then we have made our present to send to the DAC

	//okay, so now we want to assert the slave
	DAC_SS = LOW; //Select DAC line as low, inverse logic means it is on

	//now we send the important things
	spiTransceive(commandAndSelect);
	spiTransceive(dataHigh);
	spiTransceive(dataLow);

		//now we deassert the DAC
		DAC_SS = HIGH;
		DAC_SS = LOW; //I actually don't understand why we'd cycle it but ok, I will, thanks TA
		DAC_SS = HIGH;
}
;
