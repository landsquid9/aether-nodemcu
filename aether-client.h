/* This is a library designed to make it easy to use a nodeMCU to connect
 * to the Aether IoT platform http://aether.mod.bz/
 *
 * Dependencies:
 * ---- Arduino WebSockets 
 * ----------https://github.com/Links2004/arduinoWebSockets
 *
 * Messages prefixed with [AE:X] where X is log level
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

/* The variables involved in timing need to be unsigned long, because that is 
 * what millis() returns. That's a bit of a mouthful though, so let's typedef it
 */
typedef unsigned long ulong;
typedef unsigned int  uint;

/* Various data states are stored as enums.
 * Log level says how much to log to serial.
 * Data mode represents whether this devide sends or receives information.
 * Data type represents the type of data this device sends or receives.
 */
enum LOG_LEVEL
{
	LOG_NONE,
	LOG_STANDARD,
	LOG_VERBOSE
};

enum DATA_MODE
{
	MODE_SEND,
	MODE_RECEIVE
};

enum DATA_TYPE
{
	DATA_PULSE,
	DATA_BOOL,
	DATA_NUMBER,
	DATA_STRING
};

class AetherClient
{
	public:
		AetherClient();
		
		/* See LOG_LEVEL above. This defaults to verbose at the moment, so it
		 * is not necessary to call this
		 */
		void setLogVerbosity(LOG_LEVEL level);
		/* Connect to a wireless network. Give it the name of the network, the
		 * password, and an optional timeout value to say when to stop trying to
		 * connect in milliseconds. Defaults to 30000 (30 seconds);
		 */
		bool connectToAP(const char* name, const char* password, ulong wait = 30000);
		/* Connects to the aether server. Provide an identifying name for the 
		 * device, the data mode (see enum above) and the data type (see enum 
		 * above) required. If successful, you will see the device listed 
		 * on the configuration website, although the name may be different if 
		 * it was not unique
		 */
		bool connectToServer(const char* name, DATA_MODE mode, DATA_TYPE dataType);
		/* This MUST be called in the program loop */
		void loop();
		
		/* ONLY in send mode. Sends data to the server. */
		void sendData(); 			// Pulse
		void sendData(bool data);   // Boolean
		void sendData(int data);    // Number
		void sendData(float data);  // Number
		void sendData(char* data);  // String
		
		/* ONLY in receive mode. Passed a function that is called when
		 * data is sent from the server to this device. The function must
		 * return void and only have 0 or 1 arguments, of a type consistent with
		 * the data type already requested 
		 */
		void receiveData(void (*f)());
		void receiveData(void (*f)(bool));
		void receiveData(void (*f)(float));
		void receiveData(void (*f)(const char*));
		
		
	private:
		/* How much to log to Serial? */
		LOG_LEVEL log = LOG_STANDARD;
		
		/* Library objects for wifi connection and websocket connection */
		ESP8266WiFiMulti wiFiMulti;
		WebSocketsClient webSocket;
		
		/* Client info, defined by user */
		const char *clientName;
		DATA_MODE dMode;
		DATA_TYPE dType;
		
		/* Incoming message is stored here */
		String incoming;
		
		/* Is the receive function set? */
		bool funcSet = false;
		
		/* User defined function to be called when data is received */
		void(*pulseFunc)();
		void(*boolFunc)(bool);
		void(*floatFunc)(float);
		void(*stringFunc)(const char*);
		
		
		/* what happens when something is received via websocket? 
		 */
		void webSocketEvent(WStype_t type, uint8_t * payload, size_t len);
		
		/* Configure the client */
		void configure();
		

	
};