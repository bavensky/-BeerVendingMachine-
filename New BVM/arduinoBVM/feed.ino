void feedBeer(int b230, int b300, int b400) {

  //  while (state == true) {
  cup.attach(CUP);
  for (pos = 120; pos >= 30; pos -= 1) {  // yellow lcd
    //  for (pos = 30; pos <= 120; pos += 1) { // blue lcd
    cup.write(pos);
    delay(30);
  }

  Serial.println("water pump");
  delay(1000);


  if (b230 != 0) {
    while (count <= b230) {
      //      cup.write(120);  // blue lcd
      cup.write(30); // yellow lcd
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  } else if (b300 != 0) {
    while (count <= b300) {
      //      unsigned long currentMillis = millis();
      //      if (currentMillis - previousTime >= 1000) {
      //        previousTime = currentMillis;
      //        count++;
      //      }
      //      cup.write(30);
      //      digitalWrite(RELAY, LOW);

      //      cup.write(120); // blue lcd
      cup.write(30); // yellow lcd
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  } else if (b400 != 0) {
    while (count <= b400) {
      //      unsigned long currentMillis = millis();
      //      if (currentMillis - previousTime >= 1000) {
      //        previousTime = currentMillis;
      //        count++;
      //      }
      //      cup.write(30);
      //      digitalWrite(RELAY, LOW);


      //      cup.write(120); // blue lcd
      cup.write(30); // yellow lcd
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  }


  digitalWrite(RELAY, HIGH);
  count = 0;

  //  for (pos = 120; pos >= 30; pos -= 1) { // blue lcd
  for (pos = 30; pos <= 120; pos += 1) { // yellow lcd
    cup.write(pos);
    delay(30);
  }
  cup.detach();


  //    state = false;
  //  }
}
