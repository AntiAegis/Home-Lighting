/******************************************************************************
 *	Include
 *****************************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "pin_def.h"
#include "light.h"


/******************************************************************************
 *	Global
 *****************************************************************************/
/* SSID and password */
extern const char* ssid;
extern const char* password;

/* Web-server */
extern ESP8266WebServer server;


/******************************************************************************
 *	Functions
 *****************************************************************************/
/* Setup */
void serverSetup();

/* WiFi connection */
void wifiKeepConnection();

/* Server handling */
void handleRoot();
void handleNotFound();
void handleState();
void handleChange();


