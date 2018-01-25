/******************************************************************************
 *	Include
 *****************************************************************************/
#include "wifi.h"


/******************************************************************************
 *	Global
 *****************************************************************************/


/******************************************************************************
 *	Setup
 *****************************************************************************/
void setup(void){
	/* Setup pins */  
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_RELAY, OUTPUT);
	digitalWrite(PIN_LED, LOW);
	digitalWrite(PIN_RELAY, LOW);

	/* Serial setup */
	Serial.begin(115200);

	/* Setup server */
	serverSetup();
}


/******************************************************************************
 *	Loop
 *****************************************************************************/
void loop(void){
  /* Server service */
  server.handleClient();
  
  /* For calib ADC */
  while (Serial.available() > 0) {
      digitalWrite(PIN_LED, 1);
      ANALOG_BUFF_LEN = Serial.parseInt();
      ANALOG_THRES = Serial.parseInt();
      Serial.println(ANALOG_BUFF_LEN);
      Serial.println(ANALOG_THRES);
      digitalWrite(PIN_LED, 0);
  }
}

