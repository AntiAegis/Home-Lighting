/******************************************************************************
 *	Include
 *****************************************************************************/
#include "wifi.h"
#include "web_time.h"
#include "alarm.h"


/******************************************************************************
 *	Global
 *****************************************************************************/


/******************************************************************************
 *	Setup
 *****************************************************************************/
void setup(){
	/* Setup pins */  
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_RELAY, OUTPUT);
	digitalWrite(PIN_LED, LOW);
	digitalWrite(PIN_RELAY, LOW);

	/* Serial setup */
	Serial.begin(115200);

	/* Setup server */
	serverSetup();

	/* Watchdog timer */
	ESP.wdtDisable();

	/* UDP setup */
	udpSetup();
}


/******************************************************************************
 *	Loop
 *****************************************************************************/
void loop(){
  /* Server service */
  server.handleClient();

  /* Keep WiFi conenction */
  wifiKeepConnection();

  /* Get time from NTP server */
  getNTPTime();

  /* Auto on/off light based on NTP time */
  autoLight();
  
  /* Calibrate ADC parameters */
  while (Serial.available() > 0) {
      digitalWrite(PIN_LED, 1);
      ANALOG_BUFF_LEN = Serial.parseInt();
      ANALOG_THRES = Serial.parseInt();
      Serial.println(ANALOG_BUFF_LEN);
      Serial.println(ANALOG_THRES);
      digitalWrite(PIN_LED, 0);
  }

  /* Watchdog timer */
  ESP.wdtFeed();
}

