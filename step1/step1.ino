#include <ESP32Servo.h>

Servo servo;

const int STOP  = 94;
const int LEFT  = 90;
const int RIGHT = 98;

long leftSteps = 0;
long rightSteps = 0;
long position = 0;  // estimated position

void setup() {
  Serial.begin(115200);

  servo.attach(26);
  servo.write(STOP);

  Serial.println("Commands:");
  Serial.println("L = Step Left");
  Serial.println("R = Step Right");
}

void stepLeft(int pulseTimeMs) {
  servo.write(LEFT);
  delay(pulseTimeMs);

  servo.write(STOP);

  leftSteps++;
  position--;

  printStatus();
}

void stepRight(int pulseTimeMs) {
  servo.write(RIGHT);
  delay(pulseTimeMs);

  servo.write(STOP);

  rightSteps++;
  position++;

  printStatus();
}

void printStatus() {
  Serial.print("Position: ");
  Serial.print(position);

  Serial.print(" | Left Steps: ");
  Serial.print(leftSteps);

  Serial.print(" | Right Steps: ");
  Serial.println(rightSteps);
}

void loop() {

  if (Serial.available()) {

    char cmd = toupper(Serial.read());

    switch (cmd) {

      case 'L':
        stepLeft(20);
        break;

      case 'R':
        stepRight(20);
        break;

      default:
        Serial.println("Use L or R");
        break;
    }

    while (Serial.available()) {
      Serial.read();
    }
  }
}