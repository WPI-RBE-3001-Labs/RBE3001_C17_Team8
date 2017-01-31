/*
 * main.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Joe Lombardi
 */

#include "RBELib/RBELib.h" //RBELib

volatile uint8_t tot_overflow;
unsigned int change = 0x00;

unsigned long wave1 = 0;
unsigned long wave2 = 4000;

unsigned char waveUp1 = 1;

char inchar;

void initTC0(){
	//TCCR0A = 0b00000000;

	TCNT0 = 0x00; //initialize tick counts to 0

	TCCR0B |= (1 << CS01)|(1<<CS00); //start her up with scaling

	TIMSK0 |= (1 << TOIE0); //enable interrupt on overflow

	TIFR0 |= (1<<TOV0);

	tot_overflow = 0; //total amounts of overflows initialized to 0

	sei(); //enable global interrupts

}


void matLabDataCollect(void)
{
		  	  while(1)
		  	  {
		  		  //The get char debug function will return when a character is received
		  		  inchar = getCharDebug();
		  		  //Comment out this line once you have it working correctly
		  		 //printf("This line will print when a character is received from the serial connection \n\r");

		  		if (inchar == 'Q')
		  		{
		  			//Switch which print statement is commented out when your ready for matlab data collection example
		  			//matlab will buffer all characters until \n\r
		  			//printf("This will print if the character sent is an A \n\r");
		  			for(int i=0;i<=2000;i++)
		  			{
		  				int ticks = getADC(4);
		  				int anglePot = potAngle(4);
		  				int mV = potVolts(4);
		  				printf("%d,%d,%d\n\r",ticks,anglePot,mV);
		  				_delay_us(2500);
		  			}
		  	    printf("\n\r");
		  		}
		  	  }
}
void swagToothWave(int channel1, int channel2){

	if (waveUp1==1){ //going up
		if(wave1 > 4000){
			//if true, fail and start going down
			waveUp1 = 0; //start going down

			wave1 = wave1 - 40;
			wave2 = wave2 + 40;
		}
		else{
			//keep going up
			wave1 = wave1 + 20;
			wave2 = wave2 - 20;
		}
	}

	if(waveUp1==0){//going down
		if(wave1 < 50){
			//if true we've reached rock bottom
			waveUp1 = 1; //start going up

			wave1 = wave1 + 40;
			wave2 = wave2 - 40;
		}
		else{
			//keep going down
			wave1 = wave1 - 20;
			wave2 = wave2 + 20;
		}
	}

	setDAC(channel1,wave1);
	setDAC(channel2,wave2);
}



ISR(TIMER0_OVF_vect)
{
	tot_overflow++;

//
//
//	if(low){
//	PINBbits._P0 = 1; //Sets Port B Pin 4 to low
//	}
//
//	if(!low){
//	PINBbits._P0 = 0; //Sets Port B Pin 4 to high
//	TCNT0 = 0xAA;
//	}
//
//
//	low = !low;

//
//	if(getADC(4) < 0x0FF){
//		PINBbits._P0 = 1; //Sets Port B Pin 4 to low
//	}
//	else
//		PINBbits._P0 = 0; //Sets Port B Pin 4 to low
//

//	PINBbits._P4 = 0; //Sets Port B Pin 4 to high

}




int main(void){
//	DDRBbits._P4 = OUTPUT; //Set Port B Pin 4 to output
//	DDRBbits._P0 = OUTPUT;
//
//	DDRBbits._P0 = OUTPUT; //Set Port B Pin 4 to output
//	DDRBbits._P1 = OUTPUT;
//	DDRBbits._P2 = OUTPUT; //Set Port B Pin 4 to output
//	DDRBbits._P3 = OUTPUT;
//	DDRBbits._P4 = OUTPUT; //Set Port B Pin 4 to output
//	DDRBbits._P5 = OUTPUT;
//	DDRBbits._P6 = OUTPUT; //Set Port B Pin 4 to output
//	DDRBbits._P7 = OUTPUT;
//
//	DDRDbits._P0 = INPUT; //Set Port B Pin 4 to output
//	DDRDbits._P1 = INPUT;
//	DDRDbits._P2 = INPUT; //Set Port B Pin 4 to output
//	DDRDbits._P3 = INPUT;
//	DDRDbits._P4 = INPUT; //Set Port B Pin 4 to output
//	DDRDbits._P5 = INPUT;
//	DDRDbits._P6 = INPUT; //Set Port B Pin 4 to output
//	DDRDbits._P7 = INPUT;
//
////	PINBbits._P4 = 1; //Sets Port B Pin 4 to low
//

	initTC0();

	initRBELib();// SUPA, IMPORTANT, YOU GOTTA START RBELIB OR ELSE PRINT won WORK

	//initADC(4);

	//debugUSARTInit(DEFAULT_BAUD);


	initSPI();


	while(1){


//		matLabDataCollect();
//		setDAC(0,1024);
//		setDAC(1,2048);
		setDAC(2,3072);
		setDAC(3,4095);


		if(tot_overflow>0){//make a scheduler, check if overflowed

		swagToothWave(0,1); //triangle on 0 and 1 channels DAC
	//	printf("help me");

		tot_overflow = 0; //ooga booga set it to zooga
		}


//		if(PINDbits._P7 == 1){ //
//			count = 10;
//		}
//		else if(PINDbits._P6 == 1){ //
//			count = 56;
//		}
//		else if(PINDbits._P5 == 1){ //
//			count = 100;
//		}

//		if(tot_overflow >= count){
//				PINBbits._P0 = 1; //Sets Port B Pin 4 to low
//				tot_overflow = 0;
//				downcycle = (getADC(4)/1024)*count;
//		}
//
//		if(tot_overflow >= downcycle){
//			PINBbits._P0 = 0; //Sets Port B Pin 4 to high
//		}


//		printf("help");

		//programThatTestsTheButtons();

//		if(PINDbits._P0 == 0){
//			putCharDebug(0x29);
//		}
//		if(PINDbits._P7 == 0){
//			putCharDebug(0x28);
//		}


//		if(PINDbits._P0 == 0){
//			PINBbits._P3 = 1;
//		}
//		else
//		{
//			PINBbits._P3 = 0;
//		}

		//putCharDebug(0x29);
	//	print("help");

	}

return 0;

}


