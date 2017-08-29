#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
WiFiClient client;

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte LED = 2;
byte btBeer1 = 0, btBeer2 = 12, btBeer3 = 13, btBeer4 = 14;


byte beer1 = 0, beer2 = 0, beer3;
byte timeOutWIFI = 0;


//const char* ssid = "belkin.636";
//const char* password = "3eb7e66b";
const char* ssid = "tong";
const char* password = "man09min";
String apiKey = "U9UX97AKPPHR1RNK";


void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  pinMode(LED, OUTPUT);
  pinMode(btBeer1, INPUT_PULLUP);
  pinMode(btBeer2, INPUT_PULLUP);
  pinMode(btBeer3, INPUT_PULLUP);
  pinMode(btBeer4, INPUT_PULLUP);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("Beer Vending Machine");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("  Connecting WIFI   ");
    lcd.setCursor(timeOutWIFI, 3);
    lcd.print(".");

    timeOutWIFI += 1;
    if (timeOutWIFI >= 20) {
      lcd.clear();
      timeOutWIFI = 0;
    }
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  // first display
  lcd.setCursor(0, 0);
  lcd.print("Beer Vending Machine");
  lcd.setCursor(0, 1);
  lcd.print("1)230 ml. amount: ");
  if (beer1 <= 9)  lcd.print("0");
  lcd.print(beer1);
  lcd.setCursor(0, 2);
  lcd.print("2)300 ml. amount: ");
  if (beer2 <= 9)  lcd.print("0");
  lcd.print(beer2);
  lcd.setCursor(0, 3);
  lcd.print("3)400 ml. amount: ");
  if (beer3 <= 9)  lcd.print("0");
  lcd.print(beer3);

  if (digitalRead(btBeer1) == 0) {
    digitalWrite(LED, LOW);
    doHttpGet(beer1, beer2, beer3);
  } else {
    digitalWrite(LED, HIGH);
  }
  if (digitalRead(btBeer2) == 0) beer1 += 1;
  if (digitalRead(btBeer3) == 0) beer2 += 1;
  if (digitalRead(btBeer4) == 0) beer3 += 1;

  Serial.print(digitalRead(btBeer1));
  Serial.print("  ");
  Serial.print(digitalRead(btBeer2));
  Serial.print("  ");
  Serial.print(digitalRead(btBeer3));
  Serial.print("  ");
  Serial.println(digitalRead(btBeer4));
  delay(200);


  //  doHttpGet(beer1, beer2, beer3);
  //  delay(15000);

}

void doHttpGet(int Ibeer1, int Ibeer2, int Ibeer3) {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");


  http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(Ibeer1)
             + "&field2=" + String(Ibeer2) + "&field3=" + String(Ibeer3)); //HTTP


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
