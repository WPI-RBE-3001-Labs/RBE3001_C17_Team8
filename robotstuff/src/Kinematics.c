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
double link1 = 152.4;
double link2 = 124;
double yOffset = 142.4;

double degToRad(double ang)
{
	return ang*(M_PI/180);
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
	double angles[2];
	angles[0] = 0;
	angles[1] = -90;



	while(x != getPos(angles[0], angles[1])[0] && y != getPos(angles[0], angles[1])[1] && angles[0] < 90)
	{
		while(x != getPos(angles[0], angles[1])[0] && y != getPos(angles[0], angles[1])[1] && angles[1] < 90)
			{
				angles[1]++;
			}
			angles[1] = -90;
			angles[0]++;
	}
	return angles;
}
