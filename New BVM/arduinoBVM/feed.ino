// ฟังก์ชันสำหรับปั๊มเบียร์
void feedBeer(int b230, int b300, int b400) {

  //  while (state == true) {
  cup.attach(CUP);
  cup.write(90);
  //    for (pos = 120; pos >= 30; pos -= 1) {  // yellow lcd
  for (pos = 30; pos <= 120; pos += 1) { // blue lcd
    //    cup.write(pos);
    cup.write(180);
    delay(30);
  }

  Serial.println("water pump");
  delay(1000);


  if (b230 != 0) { // ปั๊มเบียร์ 230 ml
    while (count <= b230) {
      //      cup.write(120);  // blue lcd
      cup.write(90);
      //            cup.write(30); // yellow lcd
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  } else if (b300 != 0) { // ปั๊มเบียร์ 300 ml
    while (count <= b300) {
      //      cup.write(120); // blue lcd
      cup.write(90);
      //            cup.write(30); // yellow lcd
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  } else if (b400 != 0) { // ปั๊มเบียร์ 400 ml
    while (count <= b400) {
      //      cup.write(120); // blue lcd
      cup.write(90);
      //            cup.write(30); // yellow lcd
      Serial.println(count);
      digitalWrite(RELAY, LOW);
      count++;
      delay(1000);
    }
  }

  digitalWrite(RELAY, HIGH);
  count = 0;

  for (pos = 120; pos >= 30; pos -= 1) { // blue lcd
    //      for (pos = 30; pos <= 120; pos += 1) { // yellow lcd
    //    cup.write(pos);
    cup.write(0);
    delay(30);
  }
  cup.write(90);
  cup.detach();
}
