#include <ArduinoJson.h>  // เรียกใช้งานไลบรารี่การแปลงข้อมูลรูปแบบ Json
#include <SoftwareSerial.h> // เรียกใช้งานไลบรารี่จำลองการสื่อสารอนุกรม
#include <ESP8266WiFi.h> // เรียกใช้งานไลบรารี่จเชื่อมต่อไวไฟ
#include <ESP8266HTTPClient.h> // เรียกใช้งานไลบรารี่จส่งข้อมูลแบบ HTTP


SoftwareSerial chat(12, 14); // RX, TX // จำลองการสื่อสารอนุกรม
StaticJsonBuffer<200> jsonBuffer;  // สร้างบับเฟอร์สำหรับเก็บข้อมูล
WiFiClient client; // เรียกใช้งานออบเจค WiFiClient
const char* ssid = "CMMC_Sinet_2.4G"; // กำหนด ssid สำหรับเชื่อมต่อไวไฟ
const char* password = "zxc12345";  // กำหนด password สำหรับเชื่อมต่อไวไฟ
String apiKey = "4QLHRBN0XRHRD4SJ"; // กำหนด apiKey สำหรับส่งข้อมูลขึ้น Thingspeak
byte timeOutWIFI = 0;


void doHttpGet(int Ibeer1, int Ibeer2, int Ibeer3) {  // ฟังก์ชันรับค่าตัวแปร 3 ตัว เพื่อส่งข้อมูลไปยัง Thingspeak
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");


  // yellow lcd
//  http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(Ibeer1)
//             + "&field2=" + String(Ibeer2) + "&field3=" + String(Ibeer3));

    // blue lcd
    http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field4=" + String(Ibeer1)
               + "&field5=" + String(Ibeer2) + "&field6=" + String(Ibeer3));


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
  Serial.begin(9600);  // เริ่มต้นใช้งานพอร์ตอนุกรมแบบฮาร์ดแวร์
  chat.begin(4800); // เริ่มต้นใช้งานพอร์ตอนุกรมแบบซอฟแวร์
  delay(10);

  WiFi.begin(ssid, password); // เชื่อมต่อไวไฟ

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timeOutWIFI += 1;
    if (timeOutWIFI >= 20) {
      timeOutWIFI = 0;
    }
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  pinMode(LED_BUILTIN, OUTPUT); // กำหนดการแสดงผลสำหรับหลอด LED
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // หากมีการเชื่อมต่อไวไฟ
    JsonObject& slave = jsonBuffer.parseObject(chat); // เรียกใช้ออบเจคเพื่อเก็บข้อมูลที่อ่านได้จากพอร์ตอนุกรมแบบซอฟแวร์
    int small = slave["small"];
    int medium = slave["medium"];
    int large =  slave["large"];

    //    int small = slave["data"][0];
    //    int medium = slave["data"][1];
    //    int large =  slave["data"][2];

    Serial.print("parseObject : "); // แปลงข้อมูลจากออบเจค 
    Serial.print(small);
    Serial.print("  ");
    Serial.print(medium);
    Serial.print("  ");
    Serial.println(large);
    delay(500);

    if (small != 0) {
      doHttpGet(small, medium, large); // ส่งข้อมูลไปยัง Thingspeak
    }


    jsonBuffer.clear(); // เครียข้อมูลในออบเจค 

  }
  else  {
    Serial.println("connection lost, reconnect...");
    WiFi.begin(ssid, password);
    delay(500);
  }
}

