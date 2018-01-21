#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial chat(12, 14); // RX, TX
StaticJsonBuffer<200> jsonBuffer;


void setup() {
  Serial.begin(9600);
  chat.begin(4800);
  delay(10);
}

void loop() {
  JsonObject& slave = jsonBuffer.parseObject(chat);
  int data1 = slave["data"][0];
  int data2 = slave["data"][1];
  double data3 =  slave["data"][2];

  Serial.print("parseObject : ");
  Serial.print(data1);
  Serial.print("  ");
  Serial.print(data2);
  Serial.print("  ");
  Serial.println(data3, 6);
  delay(500);
  jsonBuffer.clear();
}

