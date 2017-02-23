#include "RBELib/RBELib.h" //RBELib
#include "range.h"

//unsigned char getRange(void){
//	//function returns distance in tenths of inches.
//	int ir = getADC(4);
//	if(ir < 400){return 0;}
//	else{ir = ir*-0.1333 + 93.333; if(ir>100){return 0;} else {return ir;}}
//}
unsigned int getRange(void){
	//function returns distance in tenths of inches.
	int ir = getADC(4);

	if(ir < 400)
	{
		return 0;
	}
	else
	{
		ir = ir*-0.1333 + 93.333;
		if(ir < 29 && ir >= 0)
		{
			return ir;
		}
		else
		{
			return 0;
		}

	}
}
