/*
 * main.c
 *   Created on: Jan 21, 2017
 *      Authors: Joe Lombardi, Billy Sullivan, William Godsey
 */


#include "RBELib/RBELib.h"
#include "Kinematics.h"
#include "auxiliary.h"


ISR(TIMER0_OVF_vect){tot_overflow++;}//count up tot_overflow on timer0 ovf interrupt


int main(void){

	armInitialization(); initializeButtons(); //call initialization functions

	int setPoint = 0;

	while(1){//Check buttons
			 if(PINBbits._P0 == LOW){setPoint = 90;}
		else if(PINBbits._P1 == LOW){setPoint = 0;homePos();}
		else if(PINBbits._P2 == LOW){}
		else if(PINBbits._P3 == LOW){}

		if(tot_overflow>2) //2 sets the sample rate to 109Hz
		{//make a scheduler, check if overflowed

			if(setPoint != 0){gotoAngles(setPoint,0);}//check if not going home, else drive

			printf("%ld, %ld, %d, %d, %i, %i, %i\r", encCount(0), encCount(1), potAngle(2), potAngle(3), getAccel(0), getAccel(1), getAccel(2));

		tot_overflow = 0; //reset scheduler

		}
	}

return 0;
}


