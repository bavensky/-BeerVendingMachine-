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
