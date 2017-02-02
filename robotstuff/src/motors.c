#include "RBELib/RBELib.h" //RBELib
#include "RBELib/motors.h"
#include <math.h>

void stopMotors(){
			setDAC(0,0);
			setDAC(1,0);
			setDAC(2,0);
			setDAC(3,0); //writing to zero stops all motors
}

void driveLink(int link, int dir){

	if(dir >= 0){ //if direction is positive, follow through
		if(link == 0){//if we're talking about link 0
			setDAC(0,dir); //driving link 1 at *dir* speed
			setDAC(1,0);
		}
		else{ //otherwise we're talking about link 1
			setDAC(2, dir); //driving link 1 at *dir* speed
			setDAC(3, 0);
		}
	}
	else{ //else, if direction is negative, follow through
		if(link == 0){ //if we're talking about link 0 we take it
			setDAC(0,0); //and set the inverse of the direction, because DAC
			setDAC(1,-dir); //doesn't like negative numbers
		}
		else{
			setDAC(2, 0);
			setDAC(3, -dir);
		}
	}

}

void gotoAngles(int lowerTheta, int upperTheta){
	int minZero = -10; //define limits for the 0th link, -10 and 130 degrees
	int maxZero = 130;

	int minOne = -110; //define limits for the 1st link, -110 and 90 degrees
	int maxOne = 90;

	if (lowerTheta < minZero) { //catch edge cases by making sure robot operates
		lowerTheta = minZero;   //within regular reach of the arm.
	} else if (lowerTheta > maxZero){
		lowerTheta = maxZero;
	}

	if (upperTheta < minOne) { //same for the upper link, make it rue the day it wanted
		upperTheta = minOne;   //to extend itself beyond what is physically possible
	} else if (upperTheta > maxOne){ //if it's above the max, set it to the max.
		upperTheta = maxOne;
	}


	//NOW IT GETS GOOD
	int zeroPID = calcPID(0, lowerTheta, potAngle(2));
	int onePID =  calcPID(1, upperTheta, potAngle(3));

//	printf("%d,%d,%d,%d\r\n",zeroPID,potAngle(2),onePID,potAngle(3));

	//And Finally, the part we've been waiting for
	driveLink(0, zeroPID); //now we drive the motors
	driveLink(1, -onePID);

	}
