#include "RBELib/RBELib.h" //RBELib
#include "range.h"

//unsigned char getRange(void){
//	//function returns distance in tenths of inches.
//	int ir = getADC(4);
//	if(ir < 400){return 0;}
//	else{ir = ir*-0.1333 + 93.333; if(ir>100){return 0;} else {return ir;}}
//}
//unsigned int getRange(void){
//	//function returns distance in tenths of inches.
//	int ir = getADC(4);//pick up the data in the fourth ADC slot
//
//	if(ir < 400)//If it is reading that there is nothing there, print code 42
//	{return 42;}//code 42 means there is no data
//
//	else //otherwise...
//	{
//		ir = ir*-0.12 + 93.333;//follow linearized model of sensor data
//
//		if(ir <= 35 && ir >= -35)//remove unecessary data
//		{
//
//			return ir;
//		}//return a worthwhile value
//		else{return 0;}//return a good-enough value because data is wrong
//	}
//}
//unsigned char getRange(void){
//	//function returns distance in tenths of inches.
//	int ir = getADC(4);
//	if(ir < 400){return 0;}
//	else{ir = ir*-0.1333 + 93.333; if(ir>100){return 0;} else {return ir;}}
//}

unsigned int getRange(void){
	//function returns distance in tenths of inches.
	int ir = getADC(4);//pick up the data in the fourth ADC slot

	if(ir < 400)//If it is reading that there is nothing there, print code 42
	{return 42;}//code 42 means there is no data

	else //otherwise...
	{
		ir = ir*-0.1333 + 93.333;//follow linearized model of sensor data
		if(ir < 29 && ir >= 0)//remove unecessary data
		{return ir;}//return a worthwhile value

		else
		{return 0;}//return a good-enough value because data is wrong
	}
}
