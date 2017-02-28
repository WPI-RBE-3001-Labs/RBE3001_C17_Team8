#include "RBELib/RBELib.h"

volatile uint8_t tot_overflow;//keep track of spooky interrupts

unsigned long wave1 = 0;
unsigned long wave2 = 4000;

unsigned char waveUp1 = 1;

char inchar;

void initTC0(){
	TCNT0 = 0x00; //initialize tick counts to 0
	TCCR0B |= (1 << CS01)|(1<<CS00); //start her up with scaling
	TIMSK0 |= (1 << TOIE0); //enable interrupt on overflow
	TIFR0 |= (1<<TOV0);
	tot_overflow = 0; //total amounts of overflows initialized to 0
	sei(); //enable global interrupts
}

//function initializes all the proper ports and baud and everything.
//detailed description is as follows within the function:
void armInitialization(void){
		initTC0(); //initialize clock
		initRBELib();// SUPA, IMPORTANT, YOU GOTTA START RBELIB OR ELSE PRINT won WORK

		initADC(0);//current sensor 0
		initADC(2);// potentiometer 0

		initADC(1);//current sensor 1
		initADC(3);// potentiometer 1

		debugUSARTInit(DEFAULT_BAUD); //start USART going, maybe make the initialize
									  //not include USART later.

		initSPI();//rather obvious initialization of SPI communication
		encInit(0);//initialize encoder 0

		setConst(0, 200, 0.5, 0.1); //lower motor PID constants (L, P, I, D)
		setConst(1, 200, 0.5, 0.1); //second motor PID constants

		DDRBbits._P4 = OUTPUT;//used to check frequency of interrupt
		initADC(4);//ADC that the IR sensor is on. Could make new init function
}

//function initializes four buttons on wires as in lab3
void initializeButtons(void){
	DDRBbits._P0 = INPUT;
	DDRBbits._P1 = INPUT;
	DDRBbits._P2 = INPUT;
	DDRBbits._P3 = INPUT;
	//PINBbits._P0 == LOW // how to check for being pressed
}



//void matLabDataCollect(void) //function takes Q to start it
//{
//		  	  while(1)
//		  	  {
//		  		  //The get char debug function will return when a character is received
//		  		  inchar = getCharDebug();
//		  		  //Comment out this line once you have it working correctly
//		  		 //printf("This line will print when a character is received from the serial connection \n\r");
//
//		  		if (inchar == 'Q')
//		  		{
//		  			//Switch which print statement is commented out when your ready for matlab data collection example
//		  			//matlab will buffer all characters until \n\r
//		  			//printf("This will print if the character sent is an A \n\r");
//		  			for(int i=0;i<=2000;i++)
//		  			{
//		  				int ticks = getADC(4);
//		  				int anglePot = potAngle(4);
//		  				int mV = potVolts(4);
//		  				printf("%d,%d,%d,",ticks,anglePot,mV);
//		  				_delay_us(2500);
//		  			}
//		  	    //printf("\n\r");
//		  		}
//		  	  }
//}



//void swagToothWave(int channel1, int channel2){
//
//	if (waveUp1==1){ //going up
//		if(wave1 > 4000){
//			//if true, fail and start going down
//			waveUp1 = 0; //start going down
//
//			wave1 = wave1 - 40;
//			wave2 = wave2 + 40;
//		}
//		else{
//			//keep going up
//			wave1 = wave1 + 20;
//			wave2 = wave2 - 20;
//		}
//	}
//
//	if(waveUp1==0){//going down
//		if(wave1 < 50){
//			//if true we've reached rock bottom
//			waveUp1 = 1; //start going up
//
//			wave1 = wave1 + 40;
//			wave2 = wave2 - 40;
//		}
//		else{
//			//keep going down
//			wave1 = wave1 - 20;
//			wave2 = wave2 + 20;
//		}
//	}
//
//	setDAC(channel1,wave1);
//	setDAC(channel2,wave2);
//}

//
//void readLinkAngles(void){
//	int get2 = potAngle(3);
//	int get = potAngle(2);
//	printf(", %d, %d\r",get,get2);
//}

int readThatAmperage(int link){
	int get = getADC(link);
	get = (int)(get-555)*3.77;
//	printf("%d mA\n\r",get);
	return get;
}

//Billy's awesome pointer based kinematics
//double *p;
//p = getPos(potAngle(2), potAngle(3));
//printf("%f, %f\r\n",*(p+0), *(p+1));// Code that publishes to matlab

