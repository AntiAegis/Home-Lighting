/******************************************************************************
 *	Include
 *****************************************************************************/
#include "wifi.h"


/******************************************************************************
 * Global
 *****************************************************************************/
 /* SSID and password */
const char* ssid = "Happy";
const char* password = "19630808";

/* Server object */
ESP8266WebServer server(80);

/**/
int ANALOG_BUFF_LEN = 40; // Number of samples
int ANALOG_THRES = 20;    // Threshold for light state


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
void serverSetup(){
    /* Setup */
    digitalWrite(PIN_LED, 0);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    /* Wait for connection */
    while(WiFi.status() != WL_CONNECTED){
        digitalWrite(PIN_LED, 1);
        delay(500);
        Serial.print(".");
        digitalWrite(PIN_LED, 0);
    }
    Serial.println("");
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP address: ");  Serial.println(WiFi.localIP());

    /* MDNS */
  	if(MDNS.begin("esp8266")){
  		Serial.println("MDNS responder started");
  	}

    /* Start server */
    server.on("/", handleRoot);
    server.on("/light-state", handleState);
    server.on("/light-change", handleChange);
  	server.onNotFound(handleNotFound);
  	server.begin();
  	Serial.println("HTTP server started");
}


//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void handleRoot(){
	digitalWrite(PIN_LED, 1);
	const char temp[] = 
    "HTTP/1.1 200 OK\
    Content-Type: text/html\
    Connection: close\
    Refresh: 5\
    <!doctype html>\
    <html>\
    <body><center>\
      <br />\
          <form action='light-state' method='get'>\
          <button type=submit style='width:400px; height: 80px;'>light-state</button>\
          </form>\
      <br />\
          <form action='light-change' method='get'>\
          <button type=submit style='width:400px; height: 80px;'>light-change</button>\
          </form>\
      <br />\
    </center></body>\
    </html>";

	server.send(200, "text/html", temp);
	digitalWrite(PIN_LED, 0);
}


//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void handleState(){
	/* Pre-fix */
	digitalWrite(PIN_LED, 1);
	char temp[85] = "<!doctype html><html><body><center>";

	/* Get the current state of the light */
  int min_val = 1024;
  int max_val = 0;
  int analogVal = 0;
  int arr_err_pp[4];
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
  int err_pp = (arr_err_pp[0] + arr_err_pp[1] + arr_err_pp[2] + arr_err_pp[3])/4;
  Serial.println(err_pp);
  
	if(err_pp > ANALOG_THRES)
		  strcat(temp, "light-on");
	else
		  strcat(temp, "light-off");

	/* Suffix */
	strcat(temp, "</center></body></html>");
	server.send(200, "text/html", temp);
	digitalWrite(PIN_LED, 0);
}


//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void handleChange(){
  	digitalWrite(PIN_LED, 1);
  	digitalWrite(PIN_RELAY, !digitalRead(PIN_RELAY));
    server.send(200, "text/html",
        "<!doctype html><html><body><center>light-change-ok</center></body></html>");
  	digitalWrite(PIN_LED, 0);
}


//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void handleNotFound(){
  	digitalWrite(PIN_LED, 1);
  	String message = "File Not Found\n";
  	server.send(404, "text/plain", message);
  	digitalWrite(PIN_LED, 0);
}

