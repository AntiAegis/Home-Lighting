/******************************************************************************
 *	Include
 *****************************************************************************/
#include "web_time.h"


/******************************************************************************
 * Global
 *****************************************************************************/


/******************************************************************************
 * Static
 *****************************************************************************/
/* Port for listenning UDP packets */
#define PORT  2390

/* NTP server */
IPAddress timeServerIP;
const char* ntpServerName = "0.asia.pool.ntp.org";
const int NTP_PACKET_SIZE = 48; 	// NTP time stamp is in the first 48 bytes
byte packetBuffer[ NTP_PACKET_SIZE];// Buffer for incoming and outgoing packets
WiFiUDP udp;						// UDP instance

/* Time */
const unsigned long seventyYears = 2208988800UL;
int time_hour=-1, time_minute=-1, time_second=-1;


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
void udpSetup(){
	Serial.println("Starting UDP");
	udp.begin(PORT);
	Serial.print("Local port: ");
	Serial.println(udp.localPort());
}
//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
void getNTPTime(){
  /* Static state variable */
  static unsigned int state = 0;
  if(++state == 30000000) state = 0;  // ~10 minutes
  else  return;
  
	/* Get the IP address of the NTP server */
	WiFi.hostByName(ntpServerName, timeServerIP); 
	sendNTPpacket(timeServerIP);
	delay(1000);

	/* Receive and analyze data from the server */
	int cb = udp.parsePacket();
	if(!cb){
	    Serial.println("no packet yet");
      time_hour = -1;
      time_minute = -1;
      time_second = -1;
	}
	else{
		// We've received a packet, read the data
		Serial.print("packet received, length=");
		Serial.println(cb);
		udp.read(packetBuffer, NTP_PACKET_SIZE);

		// Timestamp starts at byte 40-th and is two words long
		unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
		unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

		// Combine the two words to form NTP time (seconds since Jan 1, 1900)
		unsigned long secsSince1900 = highWord << 16 | lowWord;

		// Convert NTP time to UNIX time (starting on Jan 1, 1970)
		unsigned long epoch = secsSince1900 - seventyYears;

		// Print hour
    time_hour = ((epoch % 86400L) / 3600 + 7) % 24;
		Serial.print("The UTC time is ");
		Serial.print(time_hour);
		Serial.print(':');

		// Print minute
    time_minute = (epoch % 3600) / 60;
		if(time_minute < 10)	Serial.print('0');
		Serial.print(time_minute);
		Serial.print(':');

		// Print second
    time_second = epoch % 60;
		if(time_second < 10) Serial.print('0');
		Serial.println(time_second);
	}
}
//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
unsigned long sendNTPpacket(IPAddress& address)
{
	Serial.println("sending NTP packet...");
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(address, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}
//-----------------------------------------------------------------------------
/*
 *  Description:
 *
 *  Input:
 *
 *  Output:
 */
bool checkInterval(int h_start, int m_start, int h_end, int m_end){
    if(time_hour >= h_start && time_hour <= h_end){
        if(time_hour == h_start){
            if(time_minute >= m_start) return true;
            else return false;
        }
        if(time_hour == h_end){
            if(time_minute <= m_end) return true;
            else return false;
        }
    }
    return false;
}
//-----------------------------------------------------------------------------


