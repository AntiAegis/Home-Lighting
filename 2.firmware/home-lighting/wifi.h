/******************************************************************************
 *	Include
 *****************************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "pin_def.h"


/******************************************************************************
 *	Global
 *****************************************************************************/
extern ESP8266WebServer server;
extern int ANALOG_BUFF_LEN;
extern int ANALOG_THRES;


/******************************************************************************
 *	Functions
 *****************************************************************************/
void serverSetup();
void handleRoot();
void handleNotFound();
void handleState();
void handleChange();

