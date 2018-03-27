void rpm () {
//  NbTopsFan++;
  pulseCount++;
}

void calFlow() {
//  NbTopsFan = 0; //Set NbTops to 0 ready for calculations

  if ((millis() - oldTime) > 1000)   // Only process counters once per second
  {
    detachInterrupt(0);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL");

    valBeer = valBeer - totalMilliLitres;
    attachInterrupt(0, rpm, FALLING);
    
    totalMilliLitres = 0;
    pulseCount = 0;
  }


  //  sei();
  //  delay (1000);
  //  cli();
  //  //flow rate in L / hour = (Pulse frequency x 60) / 7.5Q
  //  Calc = (NbTopsFan * 60 / 7.5);
  //  //  Calc = (NbTopsFan * 2.25);
  //  Calc = Calc * 60;
  //  Calc = Calc / 1000;
  //  flowSum += Calc;
  Serial.print(Calc);
  Serial.print(" L/Minute  ");
  //  Serial.print(" mL/Minute  ");
  Serial.print(flowSum);
  Serial.println(" mL/Minute");
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
