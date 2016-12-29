#include <aether-client.h>

#define LED_PIN 2

AetherClient ae;
int ledState = LOW;

void receiveFunc()
{
  ledState = ledState?LOW:HIGH;
  digitalWrite(LED_PIN, ledState);
}

void setup() 
{
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  ae.setLogVerbosity(LOG_VERBOSE);

  bool connection = false;

  while(!connection)
  {
    connection = ae.connectToAP("SSID", "PASSWORD");
  }
  
  ae.connectToServer("Daniel", MODE_RECEIVE, DATA_PULSE);
  ae.receiveData(receiveFunc);

}

void loop() 
{
  ae.loop();
}
