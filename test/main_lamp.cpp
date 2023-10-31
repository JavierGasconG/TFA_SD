// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://www.shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <WiFi.h>
#include <MQTT.h>
#include <Arduino.h>
#include <Amper.h>
#include <rele.h>
#include <SPI.h>
const char ssid[] = "Esp32";
const char pass[] = "prueba123";
SPISettings settings(1000000, MSBFIRST, SPI_MODE0);
WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}

void messageReceived(String &topic, String &payload) {
  if(topic=="lampara/color"){
    Serial.println("Cambiando color a"+payload);
  }else if (topic=="lampara/encendido"){
    if(payload== "false"){
      open_rele();
      Serial.println("Desconectando frigo");
    }else if (payload=="true")
    {
      close_rele();
      Serial.println("Conectando firgo");
    }
  }else if (topic=="lampara/intensidad")
  {
    Serial.println("Cambiando intensidad a"+payload);
  }
  
}
double amper=100.0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  WiFi.begin(ssid, pass);
  ini_amper();
  ini_rele();

  client.begin("44.204.177.245",1883, net);
  client.setWill("lampara/latawill","false", true,2);

  connect();
  client.subscribe("lampara/color", 1);
  client.subscribe("lampara/intensidad", 1);

  client.subscribe("lampara/encendido", 1);
  client.publish("lampara/latawill", "true",true,1);
  client.onMessage(messageReceived);

}

void loop() {

  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
  client.onMessage(messageReceived);
  //amper=get_mA(settings);
  //client.publish("lampara/consumo", String(amper),true,1);

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    Serial.println("hello");
    client.publish("lampara/consumo", String(amper),true,1);
  }
  delay(5000);

  amper--;
  Serial.println(amper); 
  if (amper==0 ) {
    amper==1100;
  }


}