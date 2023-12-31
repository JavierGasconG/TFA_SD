
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
  if(topic=="calentador/limite"){
    Serial.println("Cambiando temperatuara limite a "+payload);
  }else if (topic=="calentador/encendido"){
    if(payload== "false"){
      open_rele();
      Serial.println("Desconectando calentador");
    }else if (payload=="true")
    {
      close_rele();
      Serial.println("Conectando calentador");
    }
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
  client.setWill("calentador/lastwill","desconectado", true,2);

  connect();
  client.subscribe("calentador/limite", 1);

  client.subscribe("calentador/encendido", 1);
  client.publish("calentador/lastwill", "conectado",true,2);
  client.onMessage(messageReceived);

}

void loop() {

  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
  client.onMessage(messageReceived);
  amper=get_mA(settings);
  client.publish("calentador/temperatura", String(amper*20),true,1);

/*
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    Serial.println("hello");
    client.publish("frigo/corriente", String(amper),true,1);
  }

  amper--;
  Serial.println(amper); 
  if (amper==0 ) {
    amper==1100;
  }
*/
  delay(5000);


}
