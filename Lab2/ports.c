
#include <RBELib/RBELib.h>
//
//
//void setPinsDir(char port, int dir, int numPins, ...)
//{
//	va_list listPointer;
//	va_start(listPointer, numPins);
//
//	__8bitreg_t* portPointer;
//	switch (port)
//	{
//	case 'A':
//		portPointer = &DDRAbits;
//		break;
//	case 'B':
//		portPointer = &DDRBbits;
//		break;
//	case 'C':
//		portPointer = &DDRCbits;
//		break;
//	case 'D':
//		portPointer = &DDRDbits;
//		break;
//	}
//
//	for (int i = 0; i < numPins; i++)
//	{
//		int pin = va_arg(listPointer, int);
//		// Set specified pins to specified dir
//		switch (pin)
//		{
//		case PORT0:
//			portPointer->_P0 = dir;
//			break;
//		case PORT1:
//			portPointer->_P1 = dir;
//			break;
//		case PORT2:
//			portPointer->_P2 = dir;
//			break;
//		case PORT3:
//			portPointer->_P3 = dir;
//			break;
//		case PORT4:
//			portPointer->_P4 = dir;
//			break;
//		case PORT5:
//			portPointer->_P5 = dir;
//			break;
//		case PORT6:
//			portPointer->_P6 = dir;
//			break;
//		case PORT7:
//			portPointer->_P7 = dir;
//			break;
//		}
//	}
//}
//
//void setPinsVal(char port, int val, int numPins, ...)
//{
//	va_list listPointer;								// pointer to the ...
//	va_start(listPointer, numPins);					// Initializing the pointer
//
//	// Find the correct port struct
//	__8bitreg_t* portPointer;
//	switch (port)
//	{
//	case 'A':
//		portPointer = &PORTAbits;
//		break;
//	case 'B':
//		portPointer = &PORTBbits;
//		break;
//	case 'C':
//		portPointer = &PORTCbits;
//		break;
//	case 'D':
//		portPointer = &PORTDbits;
//		break;
//	}
//
//	for (int i = 0; i < numPins; i++)
//	{
//		int pin = va_arg(listPointer, int);
//		// Set specified pins to specified val
//		switch (pin)
//		{
//		case PORT0:
//			portPointer->_P0 = val;
//			break;
//		case PORT1:
//			portPointer->_P1 = val;
//			break;
//		case PORT2:
//			portPointer->_P2 = val;
//			break;
//		case PORT3:
//			portPointer->_P3 = val;
//			break;
//		case PORT4:
//			portPointer->_P4 = val;
//			break;
//		case PORT5:
//			portPointer->_P5 = val;
//			break;
//		case PORT6:
//			portPointer->_P6 = val;
//			break;
//		case PORT7:
//			portPointer->_P7 = val;
//			break;
//		}
//	}
//}
//
//// Gets the value from the specified. Any error will return a value of 0b1111;
//unsigned int getPinsVal(char port, int numPins, ...)
//{
//	va_list listPointer;								// pointer to the ...
//	va_start(listPointer, numPins);					// Initializing the pointer
//
//	// Find the correct port struct
//	__8bitreg_t* portPointer;
//	switch (port)
//	{
//	case 'A':
//		portPointer = &PINAbits;
//		break;
//	case 'B':
//		portPointer = &PINBbits;
//		break;
//	case 'C':
//		portPointer = &PINCbits;
//		break;
//	case 'D':
//		portPointer = &PINDbits;
//		break;
//	default:
//		break;
//	}
//	unsigned int output = 0;
//	for (int i = 0; i < numPins; i++)
//	{
//		int pin = va_arg(listPointer, int);
//		// Set specified pins to specified val
//		switch (pin)
//		{
//		case PORT0:
//			output |= (0b00000000 | !portPointer->_P0);
//			break;
//		case PORT1:
//			output |= (0b00000000 | !portPointer->_P1) << 1;
//			break;
//		case PORT2:
//			output |= (0b00000000 | !portPointer->_P2) << 2;
//			break;
//		case PORT3:
//			output |= (0b00000000 | !portPointer->_P3) << 3;
//			break;
//		case PORT4:
//			output |= (0b00000000 | !portPointer->_P4) << 4;
//			break;
//		case PORT5:
//			output |= (0b00000000 | !portPointer->_P5) << 5;
//			break;
//		case PORT6:
//			output |= (0b00000000 | !portPointer->_P6) << 6;
//			break;
//		case PORT7:
//			output |= (0b00000000 | !portPointer->_P7) << 7;
//			break;
//		default:
//			break;
//		}
//	}
//	return output;
//
//}
