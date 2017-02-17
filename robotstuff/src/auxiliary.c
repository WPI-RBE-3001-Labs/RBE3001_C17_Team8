#include "RBELib/RBELib.h"



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
}
