/*
   Beer Vending Machine
   RMUTL Senior Project
   Date   : 27/11/2560
   ---pin Connecting---

    UART Arduino to ESP8266
      TX      11    14
      RX      10    12

    RELAY  :
      GND     GND
      SIG     9

    LCD i2c :
      GND     GND
      VCC     5V
      SDA     A4
      SCL     A5

    SERVO :
      GND     GND
      VCC     5V
      SIG     8

    Flow sensor :
      GND     2
      VCC     GND
      SDA     5V

    Switch :
      GND     GND
      BT1     4
      BT2     5
      BT3     6
      BT4     7
*/


#include <Wire.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial chat(10, 11); // RX, TX
StaticJsonBuffer<200> jsonBuffer;
JsonObject& master = jsonBuffer.createObject();
Servo cup;

#define FLOW 2

#define CUP 8

#define RELAY 9

#define BT1 4
#define BT2 5
#define BT3 6
#define BT4 7

volatile int NbTopsFan;
int Calc;

int pos = 0;
int flowLevel = 0;
boolean state = false;

byte beer230 = 0, beer300 = 0, beer400;



void rpm () {
  NbTopsFan++;
}

void calFlow() {
  NbTopsFan = 0;
  sei();
  delay (1000);
  cli();

  //(Pulse frequency x 60) / 7.5Q, = flow rate in L / hour
  Calc = (NbTopsFan * 60 / 7.5);
  Serial.print(Calc, DEC);
  Serial.println(" L/hour");
}

void sentData(int small, int medium, int large) {
  JsonObject& data = jsonBuffer.createObject();
  data["small"] = small;
  data["medium"] = medium;
  data["large"] = large;

  //  JsonArray& data = master.createNestedArray("data");
  //  data.add(small);
  //  data.add(medium);
  //  data.add(large);
  Serial.print("createNestedArray : ");
  master.printTo(Serial);
  Serial.println("");
  master.printTo(chat);
  jsonBuffer.clear();
}


void setup()  {
  Serial.begin(9600);
  chat.begin(4800);
  delay(1000);

  lcd.begin();
  lcd.backlight();

  cup.attach(CUP);
  //  cup.write(80);  // cup right
  cup.write(110); // cup left

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);

  pinMode(RELAY, OUTPUT);
  pinMode(FLOW, INPUT_PULLUP);
  attachInterrupt(0, rpm, RISING);
}

void loop() {

  // turn off relay
  digitalWrite(RELAY, HIGH);


  // main display
  lcd.print("      Welcome       ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print(" Reduce bubble Beer ");

  Serial.println("main...");
  Serial.print(digitalRead(BT1));
  Serial.print("  ");
  Serial.print(digitalRead(BT2));
  Serial.print("  ");
  Serial.print(digitalRead(BT3));
  Serial.print("  ");
  Serial.println(digitalRead(BT4));


  // manual mode
  while (digitalRead(BT1) == 0) {
    delay(100);
    Serial.println("manual modes");
    digitalWrite(RELAY, LOW);
  }


  // feed 400 ml
  if (digitalRead(BT4) == 0) {
    delay(100);

    Serial.println("button beer 400 press");
    beer400 += 1;
    sentData(beer230, beer300, beer400);

    lcd.setCursor(0, 0);
    lcd.print("             400 ml.");
    lcd.setCursor(0, 1);
    lcd.print("> Amount : ");
    if (beer400 <= 9)  lcd.print("0");
    lcd.print(beer400);
    lcd.print("      ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    state = true;
  }


  while (state == true) {
    cup.attach(CUP);
    for (pos = 110; pos >= 30; pos -= 1) {  // ช่องขวา มองจากด้านหลัง
      //      for (pos = 80; pos <= 110; pos += 1) { // ช่องซ้าย มองจากด้านหลัง
      cup.write(pos);
      delay(50);
    }

    Serial.println("water pump");
    digitalWrite(RELAY, LOW);
    cup.write(30);  // ช่องขวา มองจากด้านหลัง
    delay(5000);
    digitalWrite(RELAY, HIGH);

    //    for (int i = 0; i <= 9; i++) {
    //      NbTopsFan = 0;
    //      sei();
    //      delay(1000);
    //      cli();
    //      Calc = (NbTopsFan * 60 / 7.5);
    //      Serial.print(Calc, DEC);
    //      Serial.println(" L/hour");
    //    }



    //    for (pos = 110; pos >= 80; pos -= 1) { // ช่องซ้าย มองจากด้านหลัง
    for (pos = 30; pos <= 110; pos += 1) { // ช่องขวา มองจากด้านหลัง
      cup.write(pos);
      delay(50);
    }
    cup.detach();
    state = false;
  }
}
