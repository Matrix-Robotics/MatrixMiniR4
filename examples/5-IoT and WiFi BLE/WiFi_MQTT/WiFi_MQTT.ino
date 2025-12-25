/*
  Matrix Mini R4 WiFi MQTT Example
 * Description: Demonstrates how to using MQTT control LED and send messeage to broker.

 * Author: Anthony
 * Modified 20 Dec 2025

  www.matrixrobotics.com
*/
#include <WiFiS3.h>
#include <PubSubClient.h>
#include "MatrixMiniR4.h"

WiFiClient r4Client;
void connectWiFi(const char* ssid, const char* pass) {
  WiFi.begin(ssid, pass);
  Serial.print("WiFi Connection..");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    status = WiFi.status();
  }
  Serial.print("\nWi-Fi Connected! ");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

PubSubClient mqttClient(r4Client);
void connectMQTT(const char* broker, int port, const char* id, const char* user, const char* pass) {
  mqttClient.setServer(broker, port);
  mqttClient.setCallback(mqttCallback);
  while (!mqttClient.connected()) {
    Serial.print("MQTT Connection..");
    if (!mqttClient.connect(id, user, pass)) {
      delay(1000);
      Serial.print(".");
    } else {
      Serial.println("\nMQTT Connected! ");
    }
  }
}

unsigned long timer_1 = 0;

String mqtt_rcvTopic = "";
String mqtt_rcvMessage = "";
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  mqtt_rcvTopic = String(topic);
  mqtt_rcvMessage = "";
  for (unsigned int i = 0; i < length; i++) {
    mqtt_rcvMessage += (char)payload[i];
  }
  mqtt_rcvMessage.trim();

  Serial.print(String("TOPIC: ") + String(mqtt_rcvTopic));
  Serial.print(String(" MESG: ") + String(mqtt_rcvMessage));
  Serial.println(String(" toNum:") + String(mqtt_rcvMessage.toFloat()));
  LED_Ctrl(mqtt_rcvTopic, mqtt_rcvMessage.toFloat());
}

void LED_Ctrl(String topic, float msgNum) {
  if(topic == "matrix/r4/led1")
  {
    if(msgNum == 1)
    {
      MiniR4.LED.setColor(1, 255, 0, 0);
      MiniR4.Buzzer.Tone(NOTE_C4, 100);
    }
    else
    {
      MiniR4.LED.setColor(1, 0, 0, 0);
    }
  }
  if(topic == "matrix/r4/led2")
  {
    if(msgNum == 1)
    {
      MiniR4.LED.setColor(2, 255, 0, 0);
      MiniR4.Buzzer.Tone(NOTE_C5, 100);
    }
    else
    {
      MiniR4.LED.setColor(2, 0, 0, 0);
    }
  }
}

void setup()
{
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(9600);
  connectWiFi("WiFi", "12345678");
  connectMQTT("broker.hivemq.com", 1883, "", "", "");
  mqttClient.subscribe("matrix/r4/test1", 0);
  mqttClient.subscribe("matrix/r4/test2", 0);
  mqttClient.subscribe("matrix/r4/led1", 0);
  mqttClient.subscribe("matrix/r4/led2", 0);
}

void loop()
{
  mqttClient.loop();
  if((millis() - timer_1) > 5000)
  {
    mqttClient.publish("matrix/r4/test1", String(String("hello") + String((random(1, 10)))).c_str());
    mqttClient.publish("matrix/r4/test2", String(String("2hello") + String((random(1, 10)))).c_str());
    timer_1 = millis();
  }

}