#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


SoftwareSerial chat(12, 14); // RX, TX
StaticJsonBuffer<200> jsonBuffer;
WiFiClient client;
const char* ssid = "CMMC_Sinet_2.4G";
const char* password = "zxc12345";
String apiKey = "4QLHRBN0XRHRD4SJ";
byte timeOutWIFI = 0;


void doHttpGet(int Ibeer1, int Ibeer2, int Ibeer3) {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");


  http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(Ibeer1)
             + "&field2=" + String(Ibeer2) + "&field3=" + String(Ibeer3));


  // start connection and send HTTP header
  int httpCode = http.GET();



  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    Serial.print("[CONTENT]\n");

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}




void setup() {
  Serial.begin(9600);
  chat.begin(4800);
  delay(10);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timeOutWIFI += 1;
    if (timeOutWIFI >= 20) {
      timeOutWIFI = 0;
    }
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    JsonObject& slave = jsonBuffer.parseObject(chat);
    int small = slave["small"];
    int medium = slave["medium"];
    int large =  slave["large"];

    //  int small = slave["data"][0];
    //  int medium = slave["data"][1];
    //  int large =  slave["data"][2];

    Serial.print("parseObject : ");
    Serial.print(small);
    Serial.print("  ");
    Serial.print(medium);
    Serial.print("  ");
    Serial.println(large);
    delay(500);

    if (small != 0) {
      doHttpGet(small, medium, large);
    }


    jsonBuffer.clear();

  }
  else  {
    Serial.println("connection lost, reconnect...");
    WiFi.begin(ssid, password);
    delay(500);
  }
}

