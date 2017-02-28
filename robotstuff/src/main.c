/*
 * main.c
 *   Created on: Jan 21, 2017
 *      Authors: Joe Lombardi, Billy Sullivan, William Godsey
 */

//A project at Worcester Polytechnic Institute in partial fulfillment of
//the requirements of the course Unified Robotics III - Manipulation

#include "RBELib/RBELib.h"
#include "Kinematics.h"
#include "auxiliary.h"
#include "range.h"
#include "math.h"

ISR(TIMER0_OVF_vect){tot_overflow++;}//count up tot_overflow on timer0 ovf interrupt

int main(void){
	int state = 0;//keeps track of the state of the function

	float yHeight = 0;//Height of the block, set to 0 due to kinematics from link0
	float xOffset = 6.8;//Distance from arm to belt
	float xDist;//will hold distance of block x

	char done = 0;//a very important variable that has a self-explanatory use later

	//Note, conversion can be done as 1 tick ~ 1/100 of a second.
	unsigned int timeCycles = 0;//integer to hold total time elapsed after reset
	unsigned int timeToGrip = 1350;//constant integer to compare against for delay
	unsigned int timeToCloseGrip = 1000;//same as above, for different use
	unsigned int timeToWeigh = 500;//short delay for gripper to close
	unsigned int timeToGo = 5000;//time it takes to move to

	double theta1 = 0;//declare we are using two doubles for out working theta values
	double theta2 = 0;
	int waitTheta2 = 0;//declare we are using two integer values for later thetas
	int waitTheta1 = 0;

	armInitialization(); //call initialization functions, found in auxiliary.c
	initializeButtons(); //call initialization functions

	setServo(0,0); //set the conveyor to move forward, stay like that for duration
	setServo(1,255);  //set the gripper to open initially

	while(1){

		switch(state){//keep in state format, easier to troubleshoot and track flow

		case 0://Basically poll IR for new information, compare when recieved
				gotoAngles(waitTheta1,(waitTheta2+15));
				if(getRange() != 42){//while printing code 42, do nothing
					state = 1;
					done = 0;//make sure important variables are in correct standing
					timeCycles = 0;//reset time, in case this is second call of function
					printf("\r\n");//newline
					printf("block detected\r\n");
					while(tot_overflow<200){}//delay a bit
				}
				else printf("polling\r");
			break;

		case 1://Calculate theta1 and theta2 using IK, as well as info from IR
			//Y position will remain constant,
			//X position will change only a little bit.
			if(getRange() != 42){
			xDist = xOffset + (((float)getRange())/10) + 0.5;//math out the block dstnce
			theta1 = getAngles(xDist, yHeight)[0];//Set theta 1 lower link
			theta2 = getAngles(xDist, yHeight)[1];//Set theta 2 upper link
			printf("blocking\r");
			}
			else{done = 1;}//realize that the block is done blocking the sensor

			if(tot_overflow > 2){//100Hz PID loop
				gotoAngles((int)theta1,(int)theta2);
			}

			if(done){//if the block IS done blocking the sensor,
				state = 2;//move to next state
				//TEST set waitTheta to calculated theata instead
				//waitTheta1 = theta1;
				//waitTheta2 = theta2;
				waitTheta1 = potAngle(2);//store angles
				waitTheta2 = potAngle(3);//as direct read of the configuration
				printf("\r\n");
				printf("angles are, (%d,%d)\r\n",waitTheta1,waitTheta2);
				printf("waiting\r\n");
			}//move to next state
			break;

		case 2://Now wait for block to be in place, go to a starting position.
			if(tot_overflow>2){//PID loop
			gotoAngles(waitTheta1,(waitTheta2+15));//holding position
			tot_overflow = 0;//reset PID overflow loop

				if(timeCycles < timeToGrip){//check if enough time has elapsed
					timeCycles++;//increment time to delay
					printf(". . .\r");
				}
				else{
					state = 3;//move to grab state
					printf("\r\n");
					printf("BLOCK IN RANGE\r\n");
				}
			}
			break;

		case 3://Go to the configuration, and then...
			if(tot_overflow>2){//PID timed loop.
			gotoAngles(waitTheta1,waitTheta2);//Move to pick up configuration
			}

			//continuation condition, and initialization
			if(potAngle(3) == waitTheta2){//check if we are at the angle necessary
				state = 4;//set sail for the next state
				stopMotors();//stop the motors from twitching (may remove)
				setServo(1,255);//pinch the block

				printf("GRIP NOW!!!!\r\n");

				char grip = 1;//set char as grip, will tell when done gripping
				timeCycles = 0;//reset time
				while(grip){//while gripping, increment time count on 100Hz scheduler
					if(tot_overflow > 2){
						if(timeCycles < timeToCloseGrip){
							timeCycles++;//increment value itself
							printf(". . .\r");
							}
						else grip = 0;//once delay is over, cascade into case 4
					}
				}
			}
			break;

		case 4://Pinch it up, using servo controls through coprocessor
			printf("Gripped\r\n");

			char notThere = 1;//declare variable to wait for movement to position
			while(notThere){//while the variable is true, the arm is not there
				if(tot_overflow > 2){//PID at 100 Hz
				gotoAngles(45,-45);//move to weighing position
					if((potAngle(2) == 45) && (potAngle(3) == -45)){//check if there
						notThere=0;//if it IS there, the it is not NOT there 8^)
					}
				}
			}
			stopMotors();//stop motors to initiate weighing.
			printf("Weigh Block\r\n");

			state = 5;//Move to weighing stage
			break;

		case 5://Lift and weigh
			printf("Weighing...\r\n");
			timeCycles = 0;//Set time to zero to count up from.

			while(timeCycles < timeToWeigh){//count time while less that timeToWeigh
				if(tot_overflow>2){timeCycles++;tot_overflow = 0;}//delay increment
			}

			printf("Weighing Complete\r\n");
			state = 6;//Move to sorting stage
			break;

		case 6://decide sorting
			if(potAngle(2) < 40){state = 7;}//Heavy block if arm has not moved.
			else {state = 8;} //If arm has not moved past threshold, light block.
			break;

		case 7://Position of Heavy Block
			printf("Heavy Block\r\n");
			timeCycles = 0;//Set time counter to 0.

			while(timeCycles < timeToGo){//While moving to position
			if(tot_overflow>2){gotoAngles(0,0);timeCycles++;}//Operate PID at 100Hz
			}
			//And then move to state 9 to drop block.
			state = 9;
			break;

		case 8://Position of Light Block
			printf("Light BLock\r\n");
			state = 9;//Move to dropping case.
			break;

		case 9://Drop the block
			setServo(1,0);//open the grippers
			stopMotors();//stop function of the motors
			printf("Drop\r\n");
			printf("\r\n");
			state = 0;//Call first case, repeat function
			break;

		default://put the arm into a safe state
			state = 0;//return to the known path
			stopMotors();//stop motors from moving
			setServo(1,0);//open gripper
			printf("\r\n");
			printf("default");
			printf("\r\n");
		}
	}
return 0;
}
