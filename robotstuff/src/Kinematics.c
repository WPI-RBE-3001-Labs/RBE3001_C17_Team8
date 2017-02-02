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
double link2 = 5.5;
double xOffset = 5.625;


double * getPos(int theta1, int theta2)
{
		static double  point[2];
		point[0] = cos(theta1)*link1 + cos(theta2)*link2 + xOffset; 	//Calculate X position
		point[1] = sin(theta1)*link1 + sin(theta2)*link2;				//Calculate Y position
		
		return point; //Return array containing the X Y position of the end of arm 
		
}