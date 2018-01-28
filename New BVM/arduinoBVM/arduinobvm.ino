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
int count = 0;
byte beer230 = 0, beer300 = 0, beer400;

int flowSum = 0;



const float coeff = 7.5;
#define stepTime 1000
volatile uint16_t pulseCount;
float flowRate; // (L/min)
float flowRateMLS; // (mL/sec)
float flowCumVol; // (L or mL) Cumulative water volume which flow through flow meter.
unsigned long previousTime;
unsigned long currentTime;
unsigned long delTime;
float sumWater = 0;




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
  //  Calc = (NbTopsFan * 2.25);
  Calc = Calc * 60;
  Calc = Calc / 1000;
  flowSum += Calc;

  Serial.print(Calc);
  Serial.print(" mL/Minute  ");
  Serial.print(flowSum);
  Serial.println(" mL/Minute");
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

void feedBeer(int b230, int b300, int b400) {

  //  while (state == true) {
  cup.attach(CUP);
  for (pos = 120; pos >= 30; pos -= 3) {  // ช่องขวา มองจากด้านหลัง
    //  for (pos = 30; pos <= 120; pos += 1) { // ช่องซ้าย มองจากด้านหลัง
    cup.write(pos);
    delay(50);
  }

  Serial.println("water pump");
  delay(1000);


  if (b230 != 0) {
    while (count <= b230) {
      //      currentTime = millis(); //store millis() timing in currentTime
      //      delTime = currentTime - previousTime;
      //
      //      if (delTime >= stepTime) { //measureFlow in each stepTime. For example: each 1 second)
      //        //              measureFlow();
      //      }

      unsigned long currentMillis = millis();
      if (currentMillis - previousTime >= 1000) {
        previousTime = currentMillis;
        count++;
      }

      digitalWrite(RELAY, LOW);



      //      sei();
      //      delay (1000);
      //      cli();
      //
      //      Calc = (NbTopsFan * 60 / 7.5);
      //      Calc = Calc * 60;
      //      Calc = Calc / 1000;
      //      flowSum += Calc;
      //
      //      Serial.print(Calc);
      //      Serial.print(" mL/Minute  ");
      //      Serial.print(flowSum);
      //      Serial.println(" mL/Minute");

      //      cup.write(120);
      //      cup.write(30);
      //      Serial.println(count);


      //      digitalWrite(RELAY, LOW);
      //      count++;
      //      delay(1000);
      //      calFlow();
    }
  } else if (b300 != 0) {
    while (count <= b300) {
      //      cup.write(120);
      //      cup.write(30);
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  } else if (b400 != 0) {
    while (count <= b400) {
      //      cup.write(120);
      //      cup.write(30);
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  }


  digitalWrite(RELAY, HIGH);
  count = 0;

  //  for (pos = 120; pos >= 30; pos -= 1) { // ช่องซ้าย มองจากด้านหลัง
  for (pos = 30; pos <= 120; pos += 1) { // ช่องขวา มองจากด้านหลัง
    cup.write(pos);
    delay(50);
  }
  cup.detach();


  //    state = false;
  //  }
}


void setup()  {
  Serial.begin(9600);
  chat.begin(4800);
  delay(1000);

  lcd.begin();
  lcd.backlight();


  cup.attach(CUP);
  //    cup.write(30);  // cup right
  cup.write(120); // cup left


  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);

  pinMode(RELAY, OUTPUT);

  pinMode(FLOW, INPUT);
  digitalWrite(FLOW, HIGH);
  previousTime = 0;
  flowRate = 0;
  flowRateMLS = 0;
  flowCumVol = 0;
  pulseCount = 0;

  attachInterrupt(0, pulseCounter, FALLING);
  //  attachInterrupt(0, rpm, RISING);
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

  // feed 230 ml
  if (digitalRead(BT2) == 0) {
    delay(100);

    Serial.println("button beer 230 press");
    beer230 += 1;
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
    //    state = true;
    feedBeer(7, 0, 0);
  }


  // feed 300 ml
  if (digitalRead(BT3) == 0) {
    delay(100);

    Serial.println("button beer 300 press");
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
    //    state = true;
    feedBeer(0, 10, 0);
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
    //    state = true;

    feedBeer(0, 0, 15);
  }
}



//----------------------------measure flow function ---------------------------------------//
void measureFlow() {
  detachInterrupt(0);
  flowRate = (1000 * pulseCount / delTime) / coeff; //see above
  flowRateMLS = flowRate * 1000 / 60; //(mL/s)  ////this 1000 is unit conversion 1000 milli = 1 meter.
  flowCumVol += (flowRate / 60) * (delTime / 1000); // (L/sec)*(sec) = L  //this 1000 is unit conversion 1000 ms = 1 sec
  previousTime = currentTime;
  reportResult();
  pulseCount = 0;
  attachInterrupt(0, pulseCounter, FALLING);
}


//----------------------function to report measured result ---------------------------------//
void reportResult() {
  //  Serial.print("Flow Rate: ");
  //  Serial.print(flowRate, 2);
  //  Serial.print(" L/min"); //serial.print(variable, decimal)
  //  Serial.print("  Flow Rate: ");
  //  Serial.print(flowRateMLS, 1);
  //  Serial.print(" mL/s");
  //  Serial.print("  Flow Sum: ");
  //  sumWater += flowRateMLS;
  //  Serial.print(sumWater, 1);
  //  Serial.print(" mL/s");
  //  Serial.print("  Cumulative Vol: ");
  //  Serial.print(flowCumVol, 2);
  //  Serial.print(" L ");
  Serial.print(flowCumVol * 1000, 2);
  Serial.print(" mL");
  Serial.println("   ");
}

//------------------------------function to count pulse ------------------------------------//
void pulseCounter() {
  pulseCount++;
}
