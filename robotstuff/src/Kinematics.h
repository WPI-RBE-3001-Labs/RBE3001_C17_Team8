/*
 * Kinematics.h
 *
 *  Created on: Feb 2, 2017
 *      Author: Billy Sullivan
 */

#ifndef KINEMATICS_H_
#define KINEMATICS_H_

double degToRad(double ang);
double radToDeg(double rad);
double * getPos(int theta1, int theta2);
double * getAngles(double x, double y);


#endif /* KINEMATICS_H_ */
