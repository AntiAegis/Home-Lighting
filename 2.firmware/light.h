/******************************************************************************
 *	Include
 *****************************************************************************/
#include <ESP8266WiFi.h>
#include "pin_def.h"


/******************************************************************************
 *	Global
 *****************************************************************************/
extern int ANALOG_BUFF_LEN;   // Number of samples
extern int ANALOG_THRES;      // Threshold for light state


/******************************************************************************
 *	Functions
 *****************************************************************************/
bool getLightState();
void lightToggle();
void lightOn();
void lightOff();


