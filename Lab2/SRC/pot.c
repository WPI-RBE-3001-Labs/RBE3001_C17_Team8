#include "RBELib/RBELib.h" //RBELib
#include "RBELib/pot.h"


int potAngle(int pot){
	int val = getADC(pot);
	return ((int)(val*0.322) -90);
}

int potVolts(int pot){
	int val = getADC(pot);
	return ((int)(val*4.887)); //return millivolts
}



//sd
//144.23mm
//152.4mm
//127.54mm
