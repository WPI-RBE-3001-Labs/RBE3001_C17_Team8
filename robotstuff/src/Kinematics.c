/*
 * Kinematics.c
 *
 *  Created on: Feb 2, 2017
 *      Author: Billy Sullivan
 */

#include "RBELib/RBELib.h" 
#include <stdio.h>
#include <math.h> 


// variables are in inches, should be in mm
double link1 = 6;
double link2 = 6;
double yOffset = 3.75;

double degToRad(double ang)
{
	return ang*(M_PI/180);
}
double radToDeg(double rad)
{
	return rad*(180/M_PI);
}

double * getPos(int theta1, int theta2)
		
{
		static double point[2];
		 
		point[0] = cos(degToRad(theta1))*link1 + cos(degToRad(theta1 + theta2))*link2;  				//Calculate X position
		point[1] = sin(degToRad(theta1))*link1 + sin(degToRad(theta1 + theta2))*link2 + yOffset;		//Calculate Y position
		
		return point; //Return memory location containing the X Y position of the end of arm 
		
}

double * getAngles(double x, double y)
{
	static double angles[2];

	angles[0] =  radToDeg(atan(y/x) + acos((((pow(x, 2) + pow(y, 2)) + (pow(link1, 2) - pow(link2, 2))))/(2*link1*sqrt (pow(x, 2)+pow(y, 2))))); //Calculate  theta 1
	angles[1] = -radToDeg(acos((((pow(x, 2) + pow(y, 2)) - (pow(link1, 2) + pow(link2, 2)))) / (2*link1*link2))); //Calculate theta 2

	return angles;
}
