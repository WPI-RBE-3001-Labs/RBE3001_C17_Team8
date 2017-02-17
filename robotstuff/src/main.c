/*
 * main.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Joe Lombardi, and not Will
 */

//Billy Added This

//testimundo

#include "RBELib/RBELib.h" //RBELib
#include "Kinematics.h"
#include "auxiliary.h"


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


void matLabDataCollect(void) //function takes Q to start it
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
		  				printf("%d,%d,%d,",ticks,anglePot,mV);
		  				_delay_us(2500);
		  			}
		  	    //printf("\n\r");
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

void readLinkAngles(void){
	int get2 = potAngle(3);
	int get = potAngle(2);
	printf(", %d, %d\r",get,get2);
}

int readThatAmperage(int link){
	int get = getADC(link);
	get = (int)(get-555)*3.77;
//	printf("%d mA\n\r",get);
	return get;}

void initializeButtons(void){
	DDRBbits._P0 = INPUT;
	DDRBbits._P1 = INPUT;
	DDRBbits._P2 = INPUT;
	DDRBbits._P3 = INPUT;
	//PINBbits._P0 == LOW
}



ISR(TIMER0_OVF_vect){tot_overflow++;}
//		PINBbits._P4 = 1; //Sets Port B Pin 4 to high




int main(void){
	armInitialization();
	initializeButtons();

	int setPoint = 0;

	while(1){
		if(PINBbits._P0 == LOW){
			setPoint = 90;
			//setPoint2 = 0;
//			state = 1;
			//gotoAngles(setPoint,setPoint2);
		}else if(PINBbits._P1 == LOW){
			homePos();
			setPoint = 0;
			//setPoint = -1024;
			//setPoint2 = -30;
		} else if(PINBbits._P2 == LOW){
			//resetEncCount(0);
			//setPoint = 1024;
			//setPoint2 = -60;
		} else if(PINBbits._P3 == LOW){
			//setPoint = 4095;
			//setPoint2 = -90;
			//gotoAngles(setPoint,setPoint2);
		}


		if(tot_overflow>2) //2 sets the sample rate to 109Hz
		{//make a scheduler, check if overflowed
//			stopMotors();
//			readThatAmperage(0);
//			setDAC(1,0);
//			setDAC(0,4095);
//			readLinkAngles();
//			gotoAngles(setPoint,setPoint2);

			//PINBbits._P4 = 1; //Sets Port B Pin 4 to high
			//driveLink(1, setPoint);

//			double *p;

			//p = getPos(potAngle(2), potAngle(3));
			//printf("%f, %f\r\n",*(p+0), *(p+1));// Code that publishes to matlab

			printf("%ld, %ld, %d, %d, %i, %i, %i\r", encCount(0), encCount(1), potAngle(2), potAngle(3), getAccel(0), getAccel(1), getAccel(2));

			//printf("\t%i, \t%i, \t%i\n\r",(getAccel(0)), (getAccel(1)), (getAccel(2)));

		tot_overflow = 0; //ooga booga set it to zooga
		}
	}

return 0;

}


