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
signed int getAccel(int axis) //CONNECT SS OF ACCEL TO DDRC P0
{
	int byte1 = 0;
	int byte2 = 0;


	int gVal = 0;
	PORTDbits._P7 = LOW;
	spiTransceive(0b00000110); // Access Comamand

	byte1 = spiTransceive(axis << 6);
	byte2 = spiTransceive(0x00);
	gVal = (byte1 & 0x0F) << 8;
	gVal += byte2;
	PORTDbits._P7 = HIGH;

	gVal = (gVal-2048)*0.022;//Function from datasheet of parall

	return (gVal);


////BEGIN OF OLD CODE>>>
//	 	char chan = axis;
//		DDRDbits._P7 = OUTPUT;
//		int gVal = 0;
//		//PORTD &= ~(1<<6);
//		PINDbits._P7 = LOW;
//		spiTransceive(0b00000110); // Access Comamand
//		gVal = spiTransceive(chan << 6);
//		gVal = (gVal & 0x0F) << 8;
//		gVal += spiTransceive(0x00);
//		//gVal |= spiTransceive(0x00);
//		PINDbits._P7 = HIGH;
//		//PORTD |= (1<<6);
//
//		gVal = gVal*0.22;//Function from datasheet of parall
//
//		return (gVal);

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
	return 0; //No used in this lab
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

	selectEncoder(chan);	//Select encoder on chan
	spiTransceive(0x88);	//Select MDR0 and set to write
	spiTransceive(0x03);
	deselectEncoders();		//Deselect all encoders

	selectEncoder(chan); 	//Select encoder on chan
	spiTransceive(0x48); 	//Select MDR0 register and read
	deselectEncoders();		//Deselect all encoders
}

/**
 * @brief Reset the current count of the encoder ticks.
 * @param chan The channel to clear.
 *
 * @todo Clear the encoder count (set to 0).
 */
void resetEncCount(int chan)
{
	selectEncoder(chan);	//Select encoder on chan
	spiTransceive(0x20);	//Send commnand to encoder to clear
	deselectEncoders();   	//Deselect all encoders

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

	selectEncoder(chan); //Select the ecoder
	spiTransceive(0x60); //Read CNTR and send over MISO)

	// Read in the counts as 4 SPI bytes and combined them into 24 bit (signed int) value
	cnt = ((long) (spiTransceive(0)) << 24);
	cnt += ((long) (spiTransceive(0)) << 16);
	cnt += ((long) (spiTransceive(0)) << 8);
	cnt += spiTransceive(0);


	deselectEncoders(); //Deselect encoders

	// Inverts channel 0 to account for motor difference
	if (chan == 0) cnt = -cnt;

	return cnt;
}

void selectEncoder(int chan) //Function selects encoders, takes input of link
{
	if(chan == 0) ENCODER_SS_0 = LOW; //slave select encoder 0
	else if (chan == 1) ENCODER_SS_1 = LOW; //OR slave select encoder 1
	else return; //otherwise return
}

void deselectEncoders()
{
	ENCODER_SS_0 = HIGH; //disable both slave chips by setting high
	ENCODER_SS_1 = HIGH;
}
