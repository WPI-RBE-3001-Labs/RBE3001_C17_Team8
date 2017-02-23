/*
 * main.c
 *   Created on: Jan 21, 2017
 *      Authors: Joe Lombardi, Billy Sullivan, William Godsey
 */


#include "RBELib/RBELib.h"
#include "Kinematics.h"
#include "auxiliary.h"
#include "range.h"
#include "math.h"



ISR(TIMER0_OVF_vect){tot_overflow++;
}//count up tot_overflow on timer0 ovf interrupt

int main(void){
	int state = 0;

	float yHeight = 3;//Height of the block
	float xOffset = 6;//Distance from arm to belt
	float xDist;//will hold distance of block x.
	int xBlock;


	double theta1 =0;
	double theta2=0;
	int waitTheta2=0;
	int waitTheta1=0;

	DDRBbits._P4 = OUTPUT;
	initADC(4);

	armInitialization();
	initializeButtons(); //call initialization functions


	while(1){//Check buttons

		switch(state){

		case 0://Basically poll IR for new information, compare when recieved
				if(getRange()){state = 1; xBlock = getRange();}
			break;

		case 1://Calculate theta1 and theta2 using IK, as well as info from IR
			//Y position will remain constant,
			//X position will change only a little bit.
			xDist = xOffset + (xBlock/10) + 0.5;
			theta1 = getAngles(8, yHeight)[0];//Set theta 1 lower link
			theta2 = getAngles(8, yHeight)[1];//Set theta 2 upper link
			waitTheta2 = (int)(theta2); //make a pretty int for gotoAngles()
			waitTheta1 = (int)(theta1);//simply cast so computation is not run every cyc
			state = 2;//move to next state
			break;

		case 2://Now wait for block to be in place, go to a starting position.
			if(tot_overflow>2){
			gotoAngles(waitTheta1,(waitTheta2+15));
			tot_overflow = 0;
			}

			//FIGURE OUT HOW TO WAIT UNTIL BLOCK IN PLACE
		//	state = 3;
			break;

		case 3://Go to the thing, and then...
			if(tot_overflow>1){
			gotoAngles((int)theta1,(int)theta2);
			}
			if(potAngle(3) == (int)theta2){state=4;}//If in right place, move to grip
			break;

		case 4://Pinch it up, using servo controls through coprocessor
			setServo(0,177);
			tot_overflow = 0;
			while(tot_overflow < 100){}

			char notThere = 1;
			while(notThere){
				if(tot_overflow > 2){
				gotoAngles(45,-45);
				if((potAngle(2) == 45) && (potAngle(3) == -45)){
					notThere=0;
				}
				}
			}
			state = 5;
			break;

		case 5://Lift and weigh
			driveLink(1,1024);
			//delay some more time
			break;

		case 6://decide sorting
			if(potAngle(3) < -40){state = 7;}//heavy block if hasn't moved
			else {state = 8;} //it's a light block if it has moved
			break;

		case 7://position of Heavy Block
			if(tot_overflow>2){gotoAngles(0,0);}
			if((potAngle(2) == potAngle(3)) == 0){state = 9;}
			break;

		case 8://Position of Light Block
			state = 9;
			break;

		case 9://drop the block
			setServo(0,0);//find out commands through testing
			state = 0;
			break;

		default:
			state = 0;
		}
	//	printf("%6.3f, %6.3f\n\r", getAngles(8, 3)[0], getAngles(8, 3)[1]);

//		if(tot_overflow>2) //2 sets the sample rate to 109Hz
//		{//make a scheduler, check if overflowed
//		tot_overflow = 0; //reset scheduler
//			//printf("%d\n\r",getRange());
//
//		}
	}

return 0;
}
