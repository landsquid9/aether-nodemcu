#include "aether-client.h"

AetherClient::AetherClient()
{
	
}

void AetherClient::loop()
{
	webSocket.loop();
}

void AetherClient::setLogVerbosity(LOG_LEVEL level)
{
	log = level;
}

bool AetherClient::connectToAP(const char* name, const char* password, ulong wait)
{
	if(log != LOG_NONE)
	{
		Serial.print("[AE:1] Connecting to ");
		Serial.println(name);
	}		
	
	wiFiMulti.addAP(name, password);
	
	ulong curTime = millis();
	ulong prevTime = curTime;
	ulong delta = 0;
	ulong timer = 0;
	
	uint counter = 1;
	
    while(wiFiMulti.run() != WL_CONNECTED) 
	{
		curTime = millis();
		delta = curTime - prevTime;
		timer += delta;
		prevTime = curTime;
		
		if(log > LOG_NONE)
		{
			Serial.print("[AE:1] Connecting ");
			Serial.print(counter);
			Serial.println(" ...");
			counter ++;
		}
		
        delay(1000);
		if(timer >= wait)
		{
			if(log > LOG_NONE)
			{
				Serial.println("[AE:1] Could not connect to AP");
			}			
			return false;
		}
		
		
    }
	
	return true;
}

bool AetherClient::connectToServer(const char* name, DATA_MODE mode, DATA_TYPE dataType)
{
	clientName = name;
	dMode = mode;
	dType = dataType;
	if(log != LOG_NONE)
	{
		Serial.println("[AE:1] Connecting to Aether");
	}
	webSocket.begin("connectivity-92668.onmodulus.net", 80);
	
    webSocket.onEvent(std::bind(&AetherClient::webSocketEvent,
								this,
								std::placeholders::_1,
								std::placeholders::_2, 
								std::placeholders::_3));
}

void AetherClient::webSocketEvent(WStype_t type, uint8_t* payload, size_t len)
{
    switch(type) 
    {
        case WStype_DISCONNECTED:
		
			if(log != LOG_NONE)
			{
				Serial.println("[AE:1] Disconnected!");
			}
            
            break;
			
        case WStype_CONNECTED:
        {
				if(log != LOG_NONE)
				{
					Serial.println("[AE:1] Connected to Aether");
				}
                configure();
        }
		
        break;
		
        case WStype_TEXT:
        {
			if(funcSet)
			{
				incoming = (char*)payload;
				switch(dType)
				{
					case(DATA_PULSE):
					{
						pulseFunc();					
					}
					break;
					case(DATA_BOOL):
					{
						bool b;
						incoming == "true" ? b = true : b = false;
						boolFunc(b);
					}
					break;
					
					case(DATA_NUMBER):
					{
						floatFunc(incoming.toFloat());
					}
					break;
					
					case(DATA_STRING):
					{
						stringFunc(incoming.c_str());
					}
					break;
				}
			}				
		}
    }	
}

void AetherClient::configure()
{
	if(log > LOG_NONE)
	{
		Serial.println("[AE:1] Configuring client");
		Serial.print("[AE:1] - ");
		Serial.println(clientName);
	}
	char configuration[200] = "{\"messageType\":\"config\","
							  "\"messageContent\":"
							  "{"
							  "\"device\":\"nodeMCU\","
							  "\"name\":\"";
	strcat(configuration, clientName);
	strcat(configuration,  "\",");
	strcat(configuration, "\"mode\":");
	char *m1;
	switch(dMode)
	{
		case MODE_SEND:
		{
			m1 = "\"send\",";
			if(log > LOG_NONE)
			{
				Serial.print("[AE:1] - ");
				Serial.println("Send");
			}
		}
		break;
		
		
		case MODE_RECEIVE:
		{
			m1 = "\"receive\",";
			if(log > LOG_NONE)
			{
				Serial.print("[AE:1] - ");
				Serial.println("Receive");
			}
		}
		break;
	}
	

	
	strcat(configuration, m1);
	strcat(configuration, "\"dataType\":");
	
	char *m2;
	switch(dType)
	{
		case DATA_PULSE:
		{
			m2 = "\"pulse\"}}";
			if(log > LOG_NONE)
			{
				Serial.print("[AE:1] - ");
				Serial.println("Pulse");
			}
		}
		break;
		case DATA_BOOL:
		{
			m2 = "\"boolean\"}}";
			if(log > LOG_NONE)
			{
				Serial.print("[AE:1] - ");
				Serial.println("Boolean");
			}
		}
		break;
		case DATA_NUMBER:
		{
			m2 = "\"number\"}}";
			if(log > LOG_NONE)
			{
				Serial.print("[AE:1] - ");
				Serial.println("Number");
			}
		}
		break;
		case DATA_STRING:
		{
			m2 = "\"string\"}}";
			if(log > LOG_NONE)
			{
				Serial.print("[AE:1] - ");
				Serial.println("String");
			}
		}
		break;
	}
	

	
	strcat(configuration, m2);
	
	if(log > LOG_STANDARD)
	{
		Serial.print("[AE:2] Configuration message: ");
		Serial.println(configuration);			
	}

  
	webSocket.sendTXT(configuration);
}

void AetherClient::sendData()
{
	webSocket.sendTXT("pulse");
}

void AetherClient::sendData(bool data)
{
	char *cs;
	data ? cs = "true" : cs = "false";
	webSocket.sendTXT(cs);
}

void AetherClient::sendData(int data)
{
	webSocket.sendTXT((char*)data);
}

void AetherClient::sendData(float data)
{
	char* pC;
	dtostrf(data, 4, 3, pC);
	webSocket.sendTXT(pC);
}

void AetherClient::sendData(char* data)
{
	webSocket.sendTXT(data);
}

void AetherClient::receiveData(void (*f)())
{
	pulseFunc = f;
	funcSet = true;
}
void AetherClient::receiveData(void (*f)(bool))
{
	boolFunc = f;
	funcSet = true;
}
void AetherClient::receiveData(void (*f)(float))
{
	floatFunc = f;
	funcSet = true;
}
void AetherClient::receiveData(void (*f)(const char*))
{
	stringFunc = f;
	funcSet = true;
}