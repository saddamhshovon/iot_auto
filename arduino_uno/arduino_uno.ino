#include <ArduinoJson.h>
#include <AFMotor.h>
// #include<SoftwareSerial.h> // Use SotwareSerial if required
// SoftwareSerial link(3, 2); // RX, TX // Use SotwareSerial if required

AF_DCMotor fm_r(1);
AF_DCMotor fm_l(2);
AF_DCMotor rm_r(4);
AF_DCMotor rm_l(3);

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  // link.begin(4800); // Begin SotwareSerial

  fm_l.run(RELEASE);
  fm_r.run(RELEASE);
  rm_l.run(RELEASE);
  rm_r.run(RELEASE);
}

void loop() {

  if (Serial.available()) { // Use 'link.available()' for SotwareSerial
    DynamicJsonDocument doc(512);

    DeserializationError error = deserializeJson(doc, Serial); // use 'link' instead of 'Serial' for SoftwareSerial

    if (error == DeserializationError::Ok ) {

      int dir = doc["dir"].as<int>();
      int  fm_ls = doc["fm_l"].as<int>();
      int  fm_rs = doc["fm_r"].as<int>();
      int  rm_ls = doc["rm_l"].as<int>();
      int  rm_rs = doc["rm_r"].as<int>();

      if (dir == 1) {
        fm_l.run(FORWARD);
        fm_r.run(FORWARD);
        rm_l.run(FORWARD);
        rm_r.run(FORWARD);

        fm_l.setSpeed(fm_ls);
        fm_r.setSpeed(fm_rs);
        rm_l.setSpeed(rm_ls);
        rm_r.setSpeed(rm_rs);
      } else if (dir == 2) {
        fm_l.run(BACKWARD);
        fm_r.run(BACKWARD);
        rm_l.run(BACKWARD);
        rm_r.run(BACKWARD);

        fm_l.setSpeed(fm_ls);
        fm_r.setSpeed(fm_rs);
        rm_l.setSpeed(rm_ls);
        rm_r.setSpeed(rm_rs);
      } else {
        fm_l.run(RELEASE);
        fm_r.run(RELEASE);
        rm_l.run(RELEASE);
        rm_r.run(RELEASE);
      }

    } else {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());

      while (Serial.available() > 0) { // Use 'link' instead of 'Serial' for SoftwareSerial
        Serial.read();
      }
    }
  }
}
