#include "RBELib/RBELib.h"
#include "RBELib/ADC.h"

////STOLEN FROM THE GOLDEN CODE OF LAB 1!!!///// :^) ty


/*
 * 50khz - 200khz
 * Free running - sample constantly
 * 13 cycles a conversion - 25 1st
 * P 246 - proper changing of channels
 *
 * ADC = (VIN * 1024) / Vref
 */

void initADC(int channel){
  //Bits 7 - 6: Coupling capacitor at AREF
  //Bit 5: No left adjustment
  //Bits 4 - 0: Channel selection (0 - 7 for single ended)
  ADMUX = (0x40) | channel;

  //Bit 7: Enable ADC
  //Bit 6: Starts conversions
  //Bit 5: Auto trigger enable
  //Bit 4: Interrupt flag telling conversions are complete
  //Bit 3: Interrupt enable
  //Bits 2 - 0: Prescaler of 128 (18432000 / 128 = 144kHz)
  // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADEN);  // Enable ADC
  ADCSRA |= (1 << ADSC);  // Start A2D Conversions

  //ADCSRA = 0xFF;
  //Bit 7: Reserved
  //Bit 6: Analog Comparator Multiplexer Enable (ACME) - leave at 1
  //Bit 5 - 3: Reserved
  //Bit 2 - 0: Free running mode
  ADCSRB = 0x40;

  //Testing with port 7
  DDRA = 0x00;
}



void clearADC(int channel)

{
	//untested, BAsically never needed
	  ADMUX = (0x40) & channel;
}

unsigned short getADC(int channel)
{
// select the corresponding channel 0~7
// ANDing with '7' will always keep the value
// of 'channel' between 0 and 7
channel &= 0b00000111;  // AND operation with 7
ADMUX = (ADMUX & 0xF8)|channel;     // clears the bottom 3 bits before ORing

//test
// start single conversion
// write '1' to ADSC
ADCSRA |= (1<<ADSC);

// wait for conversion to complete
// till then, run loop continuously
while(!(ADCSRA & (1<<ADIF)));

//Clear ADIF by writing one to it
ADCSRA|=(1<<ADIF);
return (ADC);
}

void changeADC(int channel){
  //Change the channel using the same setting from initADC()
  ADMUX = (0x40) | channel;
}




//I think you can learn about this starting on page 259
//void initADC(int channel){
//
//	// Enable the ADC, allow interrupts, and set the clock division
//	ADCSRA = BIT(ADEN) | (BIT(ADSC) & 0) | (BIT(ADATE) & 0) | (BIT(ADIF) & 0) | (BIT(ADIE) & 0) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);
//
//
//	changeADC(channel);// Set the input channel
//
//
//}
//
//void clearADC(int channel)
//{
//	// Disable the ADC
//	ADCSRA &= (BIT(ADEN) & 0) | BIT(ADSC) | BIT(ADATE) | BIT(ADIF) | BIT(ADIE) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);
//
//	// Clear the data registers
//	ADCH = 0b00000000;
//	ADCL = 0b00000000;
//
//	changeADC(-1);
//}
//
//
//
//
//UINT16 getADC(int channel)
//{
//	changeADC(channel);
//	ADCSRA |= BIT(ADSC);
//	while ((ADCSRA & BIT(ADIF)) == 0)
//		;
//	return ADC;
//}
//
//
//void changeADC(int channel)
//{
//	switch (channel)
//	{
//	case 0:
//		ADMUX &= 0b11100000;
//		break;
//	case 1:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000001;
//		break;
//	case 2:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000010;
//		break;
//	case 3:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000011;
//		break;
//	case 4:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000100;
//		break;
//	case 5:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000101;
//		break;
//	case 6:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000110;
//		break;
//	case 7:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00000111;
//		break;
//	default:
//		ADMUX &= 0b11100000;
//		ADMUX |= 0b00011111;
//		break;
//	}
//
//	void inClass(){
//		ADMUX = (0<< REFS1)| //AVcc
//				(1<< REFS0)|
//				(0<< ADLAR)| //Clear left adjust register, keep in standard form
//				(0<< MUX4)| //Single ended mode
//				(0<< MUX3)|//single ended mode
//				(1<< MUX2)|//Setting for pin 7.
//				(1<< MUX1)|
//				(1<< MUX0);
//
//		ADCSRA=
//				(1<<ADEN	)|//enable DAC
//				(0<<ADSC	)|//don't start conversion yet
//				(0<<ADATE	)|//No external trigger
//				(0<<ADIF	)|//we don't want interrupts, so clear it
//				(0<<ADIE	)|//Still we are disabling interrupts
//				(1<<ADPS2	)| //Changes how long it takes for ADC to do conversion
//				(1<<ADPS1	)| //Longer means more accurate
//				(1<<ADPS0	);
//
//		ADCSRB =
//				(0<<ACME)| //no comparator
//				(0<<ADTS2)|
//				(0<<ADTS1)|//free running mode
//				(0<<ADTS0);
//
//		DIDR0 =
//				(0<< ADC7D	)| //disable pin 7 digital
//				(1<< ADC6D	)|
//				(1<< ADC5D	)|
//				(1<< ADC4D	)|
//				(1<< ADC3D	)|
//				(1<< ADC2D	)|
//				(1<< ADC1D	)|
//				(1<< ADC0D	);
//	}
//
//	void inClass2(){
//		ADCSRA = ADCSRA | (1<<ADSC);
//		while(ADCSRA & (1<<ADSC)){
//			//value = ADCL;
//			//highByte = ADCL
//
//		}
//	}
//
//}
