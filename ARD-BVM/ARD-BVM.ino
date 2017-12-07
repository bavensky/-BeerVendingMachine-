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


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


#define FLOW 2

#define CUP 6

#define RELAY 7

#define BT1 8
#define BT2 9
#define BT3 10
#define BT4 11



LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo cup;


volatile int NbTopsFan;
int Calc;

int pos = 0;
int flowLevel = 0;
boolean state = false;

byte beer1 = 0, beer2 = 0, beer3 = 0, beer4 = 0;

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

void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  cup.attach(CUP);
  cup.write(110);

  pinMode(RELAY, OUTPUT);

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);

  pinMode(FLOW, INPUT_PULLUP);
  attachInterrupt(0, rpm, RISING);

  lcd.setCursor(0, 0);
  lcd.print("Beer Vending Machine");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("  Connecting WIFI   ");
  //delay(3000);
}

void loop() {
  digitalWrite(RELAY, LOW);
  if (digitalRead(BT1) == 0) {
    delay(100);
    beer1 += 1;
    for (pos = 120; pos >= 30; pos -= 1) {
      cup.write(pos);
      delay(50);
    }
    Serial.println("state true");
    state = true;
  }

  while (state == true) {
    Serial.println("water pump");
    digitalWrite(RELAY, HIGH);
    cup.write(30);
    delay(9000);
    //    for (int i = 0; i <= 9; i++) {
    //      NbTopsFan = 0;
    //      sei();
    //      delay(1000);
    //      cli();
    //      Calc = (NbTopsFan * 60 / 7.5);
    //      Serial.print(Calc, DEC);
    //      Serial.println(" L/hour");
    //    }
    digitalWrite(RELAY, LOW);
    for (pos = 30; pos <= 120; pos += 1) {
      cup.write(pos);
      delay(50);
    }
    state = false;
  }


  // first display
  lcd.setCursor(0, 0);
  lcd.print("Beer Vending Machine");
  lcd.setCursor(0, 1);
  lcd.print("> 400 ml.           ");
  lcd.setCursor(0, 2);
  lcd.print("> Amount : ");
  if (beer1 <= 9)  lcd.print("0");
  lcd.print(beer1);
  lcd.print("      ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  delay(200);
}
