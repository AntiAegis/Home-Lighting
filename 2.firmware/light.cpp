/******************************************************************************
 *	Include
 *****************************************************************************/
#include "light.h"


/******************************************************************************
 * Global
 *****************************************************************************/
/* Analog Current sensor */
int ANALOG_BUFF_LEN = 40;	// Number of samples
int ANALOG_THRES = 20;	  // Threshold for light state


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
bool getLightState(){
	/* Declare */
	int min_val = 1024;
	int max_val = 0;
	int analogVal = 0;
	int arr_err_pp[4];

	/* Sampling and filtering */
	for(int i=0; i<4; i++){
		for(int j=0; j<ANALOG_BUFF_LEN; j++){
			analogVal = analogRead(A0);
			if(max_val < analogVal)
				max_val = analogVal;
			if(min_val > analogVal)
				min_val = analogVal;
			delay(1);
		}
		arr_err_pp[i] = max_val - min_val;
	}

	/* Make a decision */
	int err_pp = (arr_err_pp[0] + arr_err_pp[1] + arr_err_pp[2] + arr_err_pp[3])/4;
	if(err_pp > ANALOG_THRES)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void lightToggle(){
  	digitalWrite(PIN_RELAY, !digitalRead(PIN_RELAY));
}
//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void lightOn(){
	if(!getLightState())
		lightToggle();
}
//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void lightOff(){
	if(getLightState())
		lightToggle();
}
