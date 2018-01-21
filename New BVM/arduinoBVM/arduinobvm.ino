#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial chat(10, 11); // RX, TX
StaticJsonBuffer<200> jsonBuffer;

int btn_230 = 0;
int btn_300 = 0;
int btn_400 = 0;
int count230 = 0;
int count300 = 0;
int count400 = 0;


void setup()  {
  Serial.begin(9600);
  chat.begin(4800);
}

void loop() {
  JsonObject& master = jsonBuffer.createObject();
  JsonArray& data = master.createNestedArray("data");
  Serial.print("createNestedArray : ");
  data.add(count230);
  data.add(count300);
  data.add(count400);
  master.printTo(Serial);
  Serial.println("");
  master.printTo(chat);
  delay(2000);
  count230++;
  count300++;
  count400++;
  jsonBuffer.clear();
}
