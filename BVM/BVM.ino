/*
   Beer Vending Machine
   RMUTL Senior Project
   Date   : 27/11/2560
   ---pin Connecting---
    RELAY  :
      GND     GND
      SIG     13

    LCD i2c :
      GND     GND
      VCC     5V
      SDA     04
      SCL     05
    SERVO :
      GND     GND
      VCC     5V
      SIG     14
    Flow sensor :
      GND     12
      VCC     GND
      SDA     5V

    Switch :
      GND     GND
      BT1     15
      BT2     02
      BT3     00
      BT4     16
*/


#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
WiFiClient client;

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo cup;


#define BT1 0
#define BT2 2
#define BT3 15
#define BT4 16

#define FLOW 12
volatile int NbTopsFan;
int Calc;


byte beer1 = 0, beer2 = 0, beer3 = 0, beer4 = 0;
byte timeOutWIFI = 0;
boolean state = false;
int flowLevel = 0;
int pos = 0;


const char* ssid = "CMMC_Sinet_2.4G";
const char* password = "zxc12345";
//const char* ssid = "belkin.636";
//const char* password = "3eb7e66b";
//const char* ssid = "tong";
//const char* password = "man09min";
String apiKey = "4QLHRBN0XRHRD4SJ";

/*
   Servo sweep
  if (i >= 70 && state == true) {
    myservo.write(i);
    Serial.println(i);
    delay(100);
    i--;
  } else {
    state = false;
  }

  if(i <= 120 && state == false) {
    myservo.write(i);
    Serial.println(i);
    delay(100);
    i++;
  } else {
    state = true;
  }
*/


void rpm () {
  NbTopsFan++;
}


void setup() {
  Serial.begin(9600);

  cup.attach(14);
  cup.write(110);

  lcd.begin();
  lcd.backlight();

  pinMode(FLOW, INPUT);
  attachInterrupt(FLOW, rpm, RISING);


  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);

  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);


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
  //  digitalWrite(13, LOW);
  //  if (digitalRead(BT1) == 0) {
  //    Serial.println("state true");
  //    delay(100);
  //    beer1 += 1;
  //    for (pos = 110; pos >= 60; pos -= 1) {
  //      cup.write(pos);
  //      delay(50);
  //    }
  //    state = true;
  //  }
  //
  //  while (state == true) {
  //    Serial.println("water pump");
  //    digitalWrite(13, HIGH);
  //    cup.write(60);
  //    NbTopsFan = 0;
  //    sei();
  //    Serial.print("cal water ");
  //    delay (1000);
  //    cli();
  //    //(Pulse frequency x 60) / 7.5Q, = flow rate in L / hour
  //    Calc = (NbTopsFan * 60 / 7.5);
  //    Serial.print(Calc, DEC); //Prints the number calculated above
  //    Serial.println(" L/hour\r\n"); //Prints "L/hour" and returns a  new line
  //
  //    if (Calc >= 400) {
  //      Serial.println("water 400 ml");
  //      digitalWrite(13, LOW);
  //      for (pos = 60; pos <= 110; pos += 1) {
  //        cup.write(pos);
  //        delay(50);
  //      }
  //      state = false;
  //    }
  //  }
  digitalWrite(13, HIGH);
  NbTopsFan = 0;
  sei();
  Serial.print("cal water ");
  delay (1000);
  cli();
  //(Pulse frequency x 60) / 7.5Q, = flow rate in L / hour
  Calc = (NbTopsFan * 60 / 7.5);
  Serial.print(Calc, DEC); //Prints the number calculated above
  Serial.println(" L/hour\r\n"); //Prints "L/hour" and returns a  new line


  // first display
  lcd.setCursor(0, 0);
  lcd.print("Beer Vending Machine");
  lcd.setCursor(0, 1);
  lcd.print("> 400 ml. amount: ");
  if (beer1 <= 9)  lcd.print("0");
  lcd.print(beer1);
  lcd.setCursor(0, 2);
  lcd.print("Flow : ");
  lcd.print(Calc);
  lcd.print(" L/hour");
  lcd.print("     ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  delay(200);

  //  lcd.print("1)230 ml. amount: ");
  //  if (beer1 <= 9)  lcd.print("0");
  //  lcd.print(beer1);
  //  lcd.setCursor(0, 2);
  //  lcd.print("2)300 ml. amount: ");
  //  if (beer2 <= 9)  lcd.print("0");
  //  lcd.print(beer2);
  //  lcd.setCursor(0, 3);
  //  lcd.print("3)400 ml. amount: ");
  //  if (beer3 <= 9)  lcd.print("0");
  //  lcd.print(beer3);


  //  if (digitalRead(BT1) == 0) beer1 += 1;
  //  if (digitalRead(BT2) == 0) beer2 += 1;
  //  if (digitalRead(BT3) == 0) beer3 += 1;
  //  if (digitalRead(BT4) == 0) beer4 += 1;
  //
  //  if (digitalRead(BT1) == 0 || digitalRead(BT2) == 0 ||
  //      digitalRead(BT3) == 0 || digitalRead(BT4) == 0) {
  //    doHttpGet(beer1, beer2, beer3, beer4);
  //  }
  //
  //  Serial.print(digitalRead(BT1));
  //  Serial.print("  ");
  //  Serial.print(digitalRead(BT2));
  //  Serial.print("  ");
  //  Serial.print(digitalRead(BT3));
  //  Serial.print("  ");
  //  Serial.println(digitalRead(BT4));
  //    Serial.print(beer1);
  //    Serial.print("  ");
  //    Serial.print(beer2);
  //    Serial.print("  ");
  //    Serial.print(beer3);
  //    Serial.print("  ");
  //    Serial.println(beer4);
  //    delay(200);
  //    Serial.println("  ");
  //    Serial.println("  ");
}

void doHttpGet(int Ibeer1, int Ibeer2, int Ibeer3, int Ibeer4) {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");


  http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(Ibeer1)
             + "&field2=" + String(Ibeer2) + "&field3=" + String(Ibeer3) + "&field4=" + String(Ibeer4)); //HTTP


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
