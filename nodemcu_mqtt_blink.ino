/***************************************************/
//*WWW.EEFZ.IR*//
//*Instagram: @eefz.ir*//
//*Telegram: @EEFZIR*//
//*Tell: 07132361104*//
//*Mobile: 09176762585*//
//*Email:Iranarobat@gmail.com*//
/***************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LED_BUILTIN D7

#define WIFI_SSID "net name"     //Enter your net name//
#define WIFI_PASS "net password"  //Enter your net password//

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "your mqtt name"    //Enter your username in adafruit//
#define MQTT_PASS "your mqtt password"   //Enter your password in adafruit//


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);


Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/onoff");


void setup()
{
  Serial.begin(9600);


  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");


  mqtt.subscribe(&onoff);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    
    if (subscription == &onoff)
    {
      
      Serial.print("onoff: ");
      Serial.println((char*)onoff.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) onoff.lastread, "ON"))
      {
        
        digitalWrite(LED_BUILTIN,HIGH);
      }
      else
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}

void MQTT_connect() 
{
  int8_t ret;

  
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


