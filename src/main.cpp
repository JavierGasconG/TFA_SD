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

  //client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  Serial.begin(9600);
  //WiFi.begin(ssid, pass);
  ini_amper();
  ini_rele();
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  //client.begin("44.204.177.245",1883, net);
  //client.onMessage(messageReceived);
  //client.setWill("/hello","Me he hecho pupa", true,2);
  //connect();
}

void loop() {
  close_rele();
  //client.loop();
  //delay(10);  // <- fixes some issues with WiFi stability

  //if (!client.connected()) {
    //connect();
  //}

  // publish a message roughly every second.
  //if (millis() - lastMillis > 1000) {
  //  lastMillis = millis();
  //  Serial.println("hello");
  //  client.publish("/hello", "world",true,1);
  //}
  double value_amp=0.0;
  delay(5000);
  value_amp=get_mA(settings);
  Serial.println(value_amp);
  open_rele();
  value_amp=0.0;
  delay(5000);
  value_amp=get_mA(settings);
  Serial.println(value_amp);

}
