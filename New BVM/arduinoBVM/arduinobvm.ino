/*
   Beer Vending Machine
   RMUTL Senior Project
   Date   : 27/11/2560
   Date   : 28/01/2561

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

    LED Status :
      LED     12

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
#include <EEPROM.h>


//LiquidCrystal_I2C lcd(0x27, 20, 4);   // yellow lcd
LiquidCrystal_I2C lcd(0x3f, 20, 4);     // blue lcd
SoftwareSerial chat(10, 11); // RX, TX
StaticJsonBuffer<200> jsonBuffer;
JsonObject& master = jsonBuffer.createObject();
Servo cup;


#define addrBeer  19  // address EEPROM
#define FLOW      2
#define CUP       8
#define RELAY     9
#define LED       12
#define BT1       4
#define BT2       5
#define BT3       6
#define BT4       7

volatile int NbTopsFan;
int Calc;
int pos = 0;
int flowLevel = 0;
boolean state = false;
boolean stateBeer = false;
int count = 0;
byte beer230 = 0, beer300 = 0, beer400;

int flowSum = 0;

#define stepTime 1000
const float coeff = 7.5;
volatile uint16_t pulseCount;
float flowRate; // (L/min)
float flowRateMLS; // (mL/sec)
float flowCumVol; // (L or mL)
unsigned long previousTime;
unsigned long currentTime;
unsigned long delTime;
float sumWater = 0;

int countTime = 0;
float valBeer = 0;

unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;
float calibrationFactor = 4.5;

void setup()  {
  Serial.begin(9600);
  chat.begin(4800);
  delay(1000);

  lcd.begin();
  lcd.backlight();


  cup.attach(CUP);
  cup.write(30);  // blue lcd set servo to base
  //  cup.write(120); // yellow lcd set servo to base


  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);
  pinMode(FLOW, INPUT_PULLUP);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);


  pinMode(FLOW, INPUT);
  digitalWrite(FLOW, HIGH);

//  EEPROM.write(addrBeer, 30);  // set default 3 litre

  valBeer = EEPROM.read(addrBeer);
  valBeer = valBeer * 100;


}

void loop() {

  // turn off relay
  digitalWrite(RELAY, HIGH);
  countTime = 0;

  // main display
  lcd.print("      Welcome       ");
  lcd.setCursor(0, 1);
  lcd.print("  In stock : ");
  lcd.print(valBeer / 1000);
  lcd.print("L   ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print(" Reduce bubble Beer ");

  if (valBeer <= 400) {
    digitalWrite(LED, HIGH);
    stateBeer = true;
  } else {
    digitalWrite(LED, LOW);
  }

  while (stateBeer == true) {
    lcd.print("      Welcome       ");
    lcd.setCursor(0, 1);
    lcd.print("    Out of Stock    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print(" Reduce bubble Beer ");
  }


  // manual mode
  while (digitalRead(BT1) == 0) {
    delay(100);
    digitalWrite(RELAY, LOW);

    //    sentData(10, 20, 30);

    if ((millis() - oldTime) > 1000)   // Only process counters once per second
    {
      oldTime = millis();
      valBeer -= 30;
      lcd.setCursor(0, 1);
      lcd.print("  In stock : ");
      lcd.print(valBeer / 1000);
      lcd.print("L   ");
    }
  }

  //    delay(8000);  //230 ml
  //    delay(10000); //300 ml
  //    delay(12500); //400 ml

  // feed 230 ml
  if (digitalRead(BT2) == 0) {
    delay(100);
    valBeer -= 230;

    EEPROM.write(addrBeer, valBeer / 100); // update beer in stock

    beer230 += 1;
    Serial.print("beer230 = ");
    Serial.println(beer230);
    sentData(beer230, beer300, beer400);

    lcd.setCursor(0, 0);
    lcd.print("             230 ml.");
    lcd.setCursor(0, 1);
    lcd.print("> Amount : ");
    if (beer230 <= 9)  lcd.print("0");
    lcd.print(beer230);
    lcd.print("      ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");

    feedBeer(7, 0, 0);
  }




  // feed 300 ml
  if (digitalRead(BT3) == 0) {
    delay(100);
    valBeer -= 300;
    EEPROM.write(addrBeer, valBeer / 100); // update beer in stock

    beer300 += 1;
    sentData(beer230, beer300, beer400);

    lcd.setCursor(0, 0);
    lcd.print("             300 ml.");
    lcd.setCursor(0, 1);
    lcd.print("> Amount : ");
    if (beer300 <= 9)  lcd.print("0");
    lcd.print(beer300);
    lcd.print("      ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");

    feedBeer(0, 9, 0);
  }




  // feed 400 ml
  if (digitalRead(BT4) == 0) {
    delay(100);
    valBeer -= 400;
    EEPROM.write(addrBeer, valBeer / 100); // update beer in stock

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

    feedBeer(0, 0, 11);
  }


} // end loop

