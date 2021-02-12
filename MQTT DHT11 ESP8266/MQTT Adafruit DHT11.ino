#include <SimpleDHT.h>                   // Data ---> D3 VCC ---> 3V3 GND ---> GND
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// WiFi parametros
#define WLAN_SSID       "FAMILIA CARDENAS"
#define WLAN_PASS       "1222101947360979"
// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "root1995"
#define AIO_KEY         "aio_BCnb85DdsTU1WOJR3US6aVha5FVT" 
WiFiClient client;
#define ledPIN D0

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperatura = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperatura");
Adafruit_MQTT_Publish Humedad = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humedad");

int pinDHT11 = 0;
SimpleDHT11 dht11(pinDHT11);
byte hum = 0; 
byte temp = 0; 
void setup() {
  Serial.begin(115200);
  pinMode(ledPIN , OUTPUT);
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Conectando a red "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi conectado"));
  Serial.println(F("IP: "));
  Serial.println(WiFi.localIP());

  // conectado a adafruit io
  connect();

}

// conectando a adafruit.io via MQTT
void connect() {
  Serial.print(F("Conectando a Adafruit.IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("protocolo invalido")); break;
      case 2: Serial.println(F("ID rechazado")); break;
      case 3: Serial.println(F("Server invalido")); break;
      case 4: Serial.println(F("usuario/pass erroneo")); break;
      case 5: Serial.println(F("No auntentificado")); break;
      case 6: Serial.println(F("Fallo al suscribirse")); break;
      default: Serial.println(F("Coneccion fallida")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Reintentado conexion..."));
    delay(10000);
  }
  Serial.println(F("Conectado a Adafruit!"));
}

void loop() {
  if(! mqtt.ping(3)) {
    if(! mqtt.connected())
      connect();
  }
  dht11.read(&temp, &hum, NULL);
  Serial.print((int)temp); Serial.print(" *C, "); 
  Serial.print((int)hum); Serial.println(" H");
   if((int)temp>=31){
    digitalWrite(ledPIN , HIGH); 
     Serial.print("Encendido");
    }
    else{
      digitalWrite(ledPIN , LOW); 
       Serial.print("Apagado");
      }
  delay(5000);
   
   if (! Temperatura.publish(temp)) {                     //Publicando en Adafruit
      Serial.println(F("---"));
    } 
       if (! Humedad.publish(hum)) {                     //Publicando en Adafruit
      Serial.println(F("---"));
    }
    else {
      Serial.println(F("---"));
    }
}
