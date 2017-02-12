/*
 Periph.c
 *
 *  Created on: Feb 11, 2017
 *      Author: BillySullivan
 */
// Created By Billy Sullivan  2/11/2017


#include "RBELib/RBELib.h"
#include "RBELib/SPI.h"

/**
 * @brief Find the acceleration in the given axis (X, Y, Z).
 * @param  axis The axis that you want to get the measurement of.
 * @return gVal Value of  acceleration.
 *
 * @todo Create a function that is able to find the acceleration of a given axis.
 */
signed int getAccel(int axis)
{
	    DDRDbits._P6 = OUTPUT;
		signed int gVal = 0;
		PORTD &= ~(1<<6);
		spiTransceive(0b00000110);
		gVal = spiTransceive((axis << 6));
		gVal = (gVal & 0x0F) << 8;
		//gVal += spiTransceive(0x00);
		gVal |= spiTransceive(0x00);
		PORTD |= (1<<6);
		return (gVal);
}


/**
 * @brief Read an IR sensor and calculate the distance of the block.
 * @param  chan The port that the IR sensor is on.
 * @return value The distance the block is from the sensor.
 *
 * @todo Make a function that is able to get the ADC value of the IR sensor.
 */
int IRDist(int chan)
{
	return 0;
}

/**
 * @brief Initialize the encoders with the desired settings.
 * @param chan Channel to initialize.
 *
 * @todo Make a function that can setup both encoder chips on the board.
 */
void encInit(int chan)
{
	DDRC |= 0x30;
	//DDRC |= 0b00110000

	selectEncoder(chan);
	spiTransceive(0x88); //Select MDR0 and set to write
	spiTransceive(0x03);
	disableEncoders();

	selectEncoder(chan);
	spiTransceive(0x48); //Select MDR0 register and read
	disableEncoders();
}

/**
 * @brief Reset the current count of the encoder ticks.
 * @param chan The channel to clear.
 *
 * @todo Clear the encoder count (set to 0).
 */
void resetEncCount(int chan)
{
	selectEncoder(chan);
	spiTransceive(0x20);
	disableEncoders();

}

/**
 * @brief Finds the current count of one of the encoders.
 * @param  chan Channel that the encoder is on that you would like to read.
 * @return count The current count of the encoder.
 *
 * @todo Find the current encoder ticks on a given channel.
 */
signed long encCount(int chan)
{
	signed long cnt = 0;

	selectEncoder(chan);
	spiTransceive(0x60); //Read CNTR and send over MISO)

	// Shifts 4 SPI bytes into 24 bit value
	cnt = ((long) (spiTransceive(0)) << 24);
	cnt += ((long) (spiTransceive(0)) << 16);
	cnt += ((long) (spiTransceive(0)) << 8);
	cnt += spiTransceive(0);


	disableEncoders();

	// Inverts channel 0 to account for motor difference
	if (chan == 0) cnt = -cnt;

	return cnt;
}

void selectEncoder(int chan)
{
	if(chan == 0) ENCODER_SS_0 = LOW;
	else if (chan == 1) ENCODER_SS_1 = LOW;
	else return;
}

void disableEncoders()
{
	ENCODER_SS_0 = HIGH;
	ENCODER_SS_1 = HIGH;
}
