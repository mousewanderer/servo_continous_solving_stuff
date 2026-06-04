#include <ESP32Servo.h>

Servo servo;

void setup() {
  Serial.begin(115200);
  servo.attach(26);

  Serial.println("Enter value 0-180");
}

void loop() {
  if (Serial.available()) {
    int val = Serial.parseInt();

    if (val >= 0 && val <= 180) {
      servo.write(val);

      Serial.print("Sent: ");
      Serial.println(val);
    }

    while (Serial.available())
      Serial.read();
  }
}