#include "RBELib/RBELib.h" //RBELib
#include "RBELib/pot.h"


int potAngle(int pot){

	if(pot == 2){ //if lower pot, then use scaling for lower one
	int val = getADC(pot);
	return ((int)(val-273)*0.2406);
	}

	if(pot == 3){ //if lower pot, then use scaling for lower one
	int val = getADC(pot);
	return ((int)((val-273)*0.2406)-90);
	}

	else
		return 0;
}

int potVolts(int pot){
	int val = getADC(pot);
	return ((int)(val*4.887)); //return millivolts
}



//sd
//144.23mm
//152.4mm
//127.54mm
