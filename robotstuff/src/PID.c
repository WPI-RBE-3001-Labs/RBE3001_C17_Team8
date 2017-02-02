#include "RBELib/RBELib.h" //RBELib
#include "RBELib/PID.h"


int sumZero = 0; //initialize sum to zero for both
int sumOne = 0;  //we absolutely want to control it.

int setpointZeroCheck;  //int to check if we have a new setpoint
int setpointOneCheck;  //we want to reset on new setpoint

int errorPreviousZero; //int to check what our last error was
int errorPreviousOne;  //same here, on the reset that is

pidConst pidConsts; //Make a struct to hold all the delicious data





void setConst(char link, float Kp, float Ki, float Kd){
	//Alright, let's set some constants

	switch(link){ //We want to set up a switchcase to determine which link
	case 0://oh look at that we're working with the lower link
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
		break;
	case 1: //ok now we're checking out that top link
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
		break;
	}
}









signed int calcPID(char link, int setPoint, int actPos){

	int error1;
	long returnValue; //sets pidReturn as an integer

	switch(link){ //time to separate the links again
	case 0:
		if(setPoint != setpointZeroCheck){ //if, then reset sum, error, and setpoint
			sumZero = 0;
			errorPreviousZero = 0;
			setpointZeroCheck = setPoint;
		}

		error1 = setPoint - actPos;
		sumZero += error1; //OOH MAKE USE OF THAT 2049 KNOWLEDGE "+=" for days

		returnValue = (long)((pidConsts.Kp_L) * error1) 	 			 //P
					+ (pidConsts.Ki_L) * sumZero					 //I
					+ (pidConsts.Kd_L) * (error1 - errorPreviousZero);//D
					//calculates lower link PID value

		errorPreviousZero = error1; //store the error for the next iteration
		break;

	case 1:
		if(setPoint != setpointOneCheck){ //if, then reset sum, error, and setpoint
			sumOne = 0;
			errorPreviousOne = 0;
			setpointOneCheck = setPoint;
		}

		error1 = setPoint - actPos;
		sumOne += error1; //OOH MAKE USE OF THAT 2049 KNOWLEDGE "+=" for days

		returnValue = (long)((pidConsts.Kp_H) * error1)				//P
					+ (pidConsts.Ki_H) * sumOne						//I
					+ (pidConsts.Kd_H) * (error1 - errorPreviousOne);//D
					//calculates lower link PID value

		errorPreviousOne = error1; //same for the upper link, store the error.
	//	printf("%d,%d,%d,%d\r\n",pidConsts.Kp_H,pidConsts.Ki_H,pidConsts.Kd_H,returnValue);
		break;
	}



	if (returnValue > 4095) { //Put limits on the power of the PID function
		returnValue = 4095;   //if over 4095, set to 4095, and vice versa
	} else if (returnValue < -4095) {
		returnValue = -4095;
	}

	//printf("%d\r\n",returnValue);

	return returnValue; //Finally, return what we wanted to begin with...

}

