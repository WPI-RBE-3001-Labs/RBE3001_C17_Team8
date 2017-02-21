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
	int state = 0;

	armInitialization(); initializeButtons(); //call initialization functions

	while(1){//Check buttons

		switch(state){

		case 0://Basically poll IR for new information, compare when recieved

			//if(block) then state = 1;
			break;

		case 1://Calculate theta1 and theta2 using IK, as well as info from IR
			//Y position will remain constant,
			//X position will change only a little bit.

		case 2://Now wait for block to be in place, go to a starting position.
			break;

		case 3://Go to the thing, and then...
			break;

		case 4://Pinch it up, using servo controls through coprocessor
			setServo(0,0);
			break;

		case 5://Lift and weigh
			break;

		case 6://decide sorting
			break;

		case 7://position of Heavy Block
			break;

		case 8://Position of Light Block
			break;

		case 9://drop the block
			setServo(0,0);//find out commands through testing
			break;

		default:
			state = 0;
		}




		if(tot_overflow>2) //2 sets the sample rate to 109Hz
		{//make a scheduler, check if overflowed
		tot_overflow = 0; //reset scheduler

		}
	}

return 0;
}
