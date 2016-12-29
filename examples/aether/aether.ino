/*  This is a simple example that turns an LED on or off when it receives 
 *  a "pulse" message. A pulse is essentially an empty message; all that 
 *  matters is that we received a message to "do something". The sender
 *  could have been a button, as an example.
 */

#include <aether-client.h>

#define LED_PIN 2

/* Start the aether client */
AetherClient ae;

/* To keep track of whether the LED should be on or off */
int ledState = LOW;

/* As a receiver device, we must define a function to
 *  be called upon receipt of a message
 */
void receiveFunc()
{
  ledState = ledState?LOW:HIGH;
  digitalWrite(LED_PIN, ledState);
}

void setup() 
{
  /* You must either begin serial (any baud rate), or set 
   *  log verbosity to LOG_NONE
   */
  Serial.begin(115200);

  /* init LED */
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  /* There are three levels:
   *  LOG_NONE (no output, do not need to called Serial.begin())
   *  LOG_STANDARD (set by default, some logging)
   *  LOG_VERBOSE (maximum logging)
   */
  ae.setLogVerbosity(LOG_VERBOSE);

  /* Stores whether we are connected to an access point yet */
  bool connection = false;

  /* while we are not connected to an access point */
  while(!connection)
  {
    /* Try connecting. You must provide the name of the 
     *  wireless network, and its password. You may
     *  provide an optional paramter specifying how 
     *  many milliseconds to wait before aborting the
     *  connecting attempt (defualt is 30000 milliseconds)
     */
    connection = ae.connectToAP("SSID", "PASSWORD");
    // connection = ae.connectToAP("SSID", "PASSWORD", 20000);
  }

  /* Connect to the Aether server. You must provide a 
   *  name for the device, a mode, and a data type.
   *  Possible modes are:
   *  MODE_RECEIVE, MODE_SEND
   *  Possible data types are:
   *  DATA_PULSE,
   *  DATA_BOOL,
   *  DATA_NUMBER,
   *  DATA_STRING
   */
  ae.connectToServer("Daniel", MODE_RECEIVE, DATA_PULSE);
  /* If you have set the device to receive data, you MUST
   *  provide a function to be called upon receipt of a 
   *  message. This MUST return void, and take a single argument
   *  consistent with the data type. The argumnet may be:
   *  void - for pulse
   *  bool - for boolean
   *  float - for number
   *  const char* - for string
   */
  ae.receiveData(receiveFunc);

}

void loop() 
{
  /* You need to call loop within the loop */
  ae.loop();
}
