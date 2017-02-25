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

	float yHeight = 0;//Height of the block, set to 0 due to kinematics from link0
	float xOffset = 6.8;//Distance from arm to belt
	float xDist;//will hold distance of block x

	char done = 0;
	unsigned int timeCycles = 0;
	unsigned int timeToGrip = 1000;
	unsigned int timeToCloseGrip = 1000;
	unsigned int timeToWeigh = 10000;
	unsigned int timeToGo = 5000;


	double theta1 =0;
	double theta2=0;
	int waitTheta2=0;
	int waitTheta1=0;

	DDRBbits._P4 = OUTPUT;
	initADC(4);

	armInitialization();
	initializeButtons(); //call initialization functions

	setServo(0,177);
	setServo(1,0);

	while(1){//Check buttons

		//printf("%6.2f\n\r", getRange()*.1);

//		xDist = xOffset + (((float)getRange())/10) + 0.5;
//		theta1 = getAngles(xDist, yHeight)[0];//Set theta 1 lower link
//		theta2 = getAngles(xDist, yHeight)[1];//Set theta 2 upper link
//		printf("%i, %i, %f\n\r", (int)theta1, (int)theta2, xDist);
//
//		if(tot_overflow>2){
//		gotoAngles((int)theta1,(int)theta2+15);
//		tot_overflow = 0;
//		}
		//gotoAngles(0,0);



		switch(state){

		case 0://Basically poll IR for new information, compare when recieved
				if(getRange() != 42){
					state = 1;
					done = 0;
					timeCycles = 0;
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
			xDist = xOffset + (((float)getRange())/10) + 0.5;
			theta1 = getAngles(xDist, yHeight)[0];//Set theta 1 lower link
			theta2 = getAngles(xDist, yHeight)[1];//Set theta 2 upper link
			printf("blocking\r");
			}
			else{done = 1;}

			if(tot_overflow > 2){
				gotoAngles((int)theta1,(int)theta2);
			}

			if(done){state = 2;
			waitTheta1 = potAngle(2);
			waitTheta2 = potAngle(3);
			printf("\r\n");
			printf("angles are, (%d,%d)\r\n",waitTheta1,waitTheta2);
			printf("waiting\r\n");

			}//move to next state
			break;

		case 2://Now wait for block to be in place, go to a starting position.
			if(tot_overflow>2){
			gotoAngles(waitTheta1,(waitTheta2+15));//holding position
			tot_overflow = 0;//reset PID overflow loop

			if(timeCycles < timeToGrip){
				timeCycles++;
				printf(". . .\r");
			}
			else{
				state = 3;
				printf("\r\n");
				printf("BLOCK IN RANGE\r\n");
			}

			}
			break;
//
//			//FIGURE OUT HOW TO WAIT UNTIL BLOCK IN PLACE
//		//	state = 3;
//			break;
//
		case 3://Go to the thing, and then...
			if(tot_overflow>2){
			gotoAngles(waitTheta1,waitTheta2);
			}

			//continuation condition, and initialization
			if(potAngle(3) == waitTheta2){//check if we are at the angle necessary
				state = 4;//set sail for the next state
				stopMotors();//stop the motors from twitching (may remove)
				//setServo(0,69);//pinch the block

				printf("GRIP NOW!!!!\r\n");

				char grip = 1;
				timeCycles = 0;
				while(grip){
					if(tot_overflow > 2){
						if(timeCycles < timeToCloseGrip){
							timeCycles++;
							printf(". . .\r");
				}
						else grip = 0;
				}
				}
			}
			break;

		case 4://Pinch it up, using servo controls through coprocessor
			printf("Gripped\r\n");

			char notThere = 1;
			while(notThere){
				if(tot_overflow > 2){
				gotoAngles(45,-45);
				if((potAngle(2) == 45) && (potAngle(3) == -45)){
					notThere=0;
				}
				}
			}
			stopMotors();
			printf("Weigh Block\r\n");

			state = 5;
			break;

		case 5://Lift and weigh
			printf("Weighing...\r\n");
			//driveLink(1,1024);
			//delay some more time
			timeCycles = 0;
			while(timeCycles < timeToWeigh){
				if(tot_overflow>2){timeCycles++;}
				}


			printf("Weighing Complete\r\n");
			state = 6;
			break;

		case 6://decide sorting
			if(potAngle(3) < -40){state = 7;}//heavy block if hasn't moved
			else {state = 8;} //it's a light block if it has moved
			break;

		case 7://position of Light Block
			printf("Light Block\r\n");

			timeCycles = 0;

			while(timeCycles < timeToGo){
			if(tot_overflow>2){gotoAngles(0,0);timeCycles++;}
			}
			state = 9;
			break;

		case 8://Position of Heavy Block
			printf("Heavy BLock\r\n");
			state = 9;
			break;

		case 9://drop the block
		//	setServo(0,0);//find out commands through testing
			stopMotors();
			printf("Drop\r\n");
			printf("\r\n");
			state = 0;
			break;

		default:
			state = 0;
			printf("\r\n");
			printf("default");
			printf("\r\n");
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
