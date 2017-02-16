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

void readLinkAngles(void){
	int get2 = potAngle(3);
	int get = potAngle(2);
	printf(", %d, %d\r",get,get2);

}

int readThatAmperage(int link){
	int get = getADC(link);

	get = (int)(get-555)*3.77;
//	printf("%d mA\n\r",get);
	return get;
}


ISR(TIMER0_OVF_vect)
{
	tot_overflow++;
//		PINBbits._P4 = 1; //Sets Port B Pin 4 to high
}




int main(void){



	DDRBbits._P4 = OUTPUT; //Set Port B Pin 4 to output
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


//	TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
//	sei(); //  Enable global interrupts
//	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
//	OCR1A = (23040); // Set CTC compare value with a prescaler of 8 (100Hz)
//	TCCR1B |= (1 << CS11); // Start timer at Fcpu/8

	initTC0(); //initialize clock

	initRBELib();// SUPA, IMPORTANT, YOU GOTTA START RBELIB OR ELSE PRINT won WORK

	initADC(0);//current sensor 0
	initADC(2);// potentiometer 0

	initADC(1);//current sensor 1
	initADC(3);// potentiometer 1
	initADC(4);

	debugUSARTInit(DEFAULT_BAUD);

	initSPI();
	encInit(0);

	setConst(0, 200, 0.5, 0.1);
	setConst(1, 200, 0.5, 0.1); //I don't want it to run right now, set to 0


	int state = 1;

	int setPoint = 0;
	int setPoint2 = 0;
//	int volts = 0;
//	int current = 0;

	DDRBbits._P0 = INPUT;
	DDRBbits._P1 = INPUT;
	DDRBbits._P2 = INPUT;
	DDRBbits._P3 = INPUT;


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

//		switch (state)
//		{
//			case 1:
//				setPoint = 0;
//				setPoint2 = 0;
//		//		printf("point 0 reached!");
//				break;
//			case 2:
//				setPoint = 0;
//				setPoint2 = -5;
//				break;
//			case 3:
//				setPoint = 20;
//				setPoint2 = -45;
//				break;
//			case 4:
//				setPoint = 18;
//				setPoint2 = -50;
//				break;
//			case 5:
//				setPoint = 35;
//				setPoint2 = -90;
//			//	printf("point 1 reached!");
//				break;
//			case 6:
//				setPoint = 35;
//				setPoint2 = -75;
//		//		printf("point 2 reached!");
//				break;
//			case 7:
//				setPoint = 35;
//				setPoint2 = -60;
//				break;
//			case 8:
//				setPoint = 15;
//				setPoint2 = -30;
//				break;
//			case 9:
//				state = 1;
//		}
//
//		if(potAngle(2)==setPoint && potAngle(3)==setPoint2)
//		{
//			state++;
//		}


//		matLabDataCollect();

//		setDAC(2,3072);
//		setDAC(3,4095);

		//printf("%ld, %i, %i, %i\n\r", encCount(1), getAccel(0), getAccel(1), getAccel(2));

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

			gotoAngles(setPoint,0);


			// = getPos(potAngle(2), potAngle(3));
			//printf("%f, %f\r\n",*(p+0), *(p+1));// Code that publishes to matlab
			//printf("%d, %d, %f, %f \n\r" ,potAngle(2), potAngle(3),  *(p+0), *(p+1));
		//	gotoAngles(setPoint,setPoint2);

			printf("%ld, %ld, %d, %d, %i, %i, %i\r", encCount(0), encCount(1), potAngle(2), potAngle(3), getAccel(0), getAccel(1), getAccel(2));

			//printf("\t%i, \t%i, \t%i\n\r",(getAccel(0)), (getAccel(1)), (getAccel(2)));


			//printf("%i \n\r", getAccel(0));
			//printf("%i, %i\n\r", getAccel(0), (encCount(0)/(15136/360)));
			//printf("%i\n\r",encCount(1));

	//	swagToothWave(0,1); //triangle on 0 and 1 channels DAC
	//	printf("help me");

		//	if(potAngle(2) != setPoint){
		//printf("%d, %d, %d\r\n",setPoint, potAngle(2), readThatAmperage(0));
		//	}

		tot_overflow = 0; //ooga booga set it to zooga


		}
	}

return 0;

}


