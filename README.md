# servo_continous_solving_stuff

in my servo neutral band is at 92 to 96 


first servo 1
at 91 slow moving at 0 max speed at left
if 97 slow moving at 180 max speed at the right



2nd servo 2 (same thing)
at 91 slow moving at 0 max speed at left
if 97 slow moving at 180 max speed at the right


so I set variables
cause 94 = (96+92)/2
consider 92 93 94 95 96
seperation buy 14 pieces cause 
91 starts spining but slowly small difference affected. 
between the 92. 1 battery then the entrie thing breaks. problem is there is no feeedback

const int STOP  = 94;
const int LEFT  = 80;  
const int RIGHT = 108;


#include <ESP32Servo.h>

Servo servo;

// ===== SERVO CALIBRATION =====
const int STOP  = 94;
const int LEFT  = 90;
const int RIGHT = 98;

// ===== STEP CALIBRATION =====
// Measured values:
// 40 steps ≈ 90°
// 80 steps ≈ 180°
// 160 steps ≈ 360°

const int STEPS_90  = 20;
const int STEPS_180 = 40;
const int STEPS_360 = 80;

// ===== TIMING =====
const int STEP_PULSE_MS = 20;
const int STEP_PAUSE_MS = 100;

// ===== COUNTERS =====
long leftSteps = 0;
long rightSteps = 0;
long position = 0; // Estimated position

void setup() {
  Serial.begin(115200);

  servo.attach(26);
  servo.write(STOP);

  Serial.println();
  Serial.println("=================================");
  Serial.println("Continuous Servo Step Controller");
  Serial.println("=================================");
  Serial.println("Commands:");
  Serial.println("L = Turn Left 90 deg");
  Serial.println("R = Turn Right 90 deg");
  Serial.println("A = Turn Left 180 deg");
  Serial.println("D = Turn Right 180 deg");
  Serial.println("Q = Turn Left 360 deg");
  Serial.println("E = Turn Right 360 deg");
  Serial.println("S = Stop");
  Serial.println("P = Print Status");
  Serial.println("=================================");
}

void printStatus() {
  Serial.println();
  Serial.println("----- STATUS -----");

  Serial.print("Estimated Position: ");
  Serial.println(position);

  Serial.print("Left Steps: ");
  Serial.println(leftSteps);

  Serial.print("Right Steps: ");
  Serial.println(rightSteps);

  Serial.println("------------------");
}

void stepLeft() {
  servo.write(LEFT);
  delay(STEP_PULSE_MS);

  servo.write(STOP);
  delay(STEP_PAUSE_MS);

  leftSteps++;
  position--;
}

void stepRight() {
  servo.write(RIGHT);
  delay(STEP_PULSE_MS);

  servo.write(STOP);
  delay(STEP_PAUSE_MS);

  rightSteps++;
  position++;
}

void moveLeftSteps(int steps) {

  Serial.print("Moving Left ");
  Serial.print(steps);
  Serial.println(" steps");

  for (int i = 0; i < steps; i++) {
    stepLeft();
  }

  printStatus();
}

void moveRightSteps(int steps) {

  Serial.print("Moving Right ");
  Serial.print(steps);
  Serial.println(" steps");

  for (int i = 0; i < steps; i++) {
    stepRight();
  }

  printStatus();
}

void loop() {

  if (Serial.available()) {

    char cmd = toupper(Serial.read());

    switch (cmd) {

      case 'L':
        moveLeftSteps(STEPS_90);
        break;

      case 'R':
        moveRightSteps(STEPS_90);
        break;

      case 'A':
        moveLeftSteps(STEPS_180);
        break;

      case 'D':
        moveRightSteps(STEPS_180);
        break;

      case 'Q':
        moveLeftSteps(STEPS_360);
        break;

      case 'E':
        moveRightSteps(STEPS_360);
        break;

      case 'S':
        servo.write(STOP);
        Serial.println("Servo Stopped");
        break;

      case 'P':
        printStatus();
        break;

      case '\n':
      case '\r':
        break;

      default:
        Serial.println("Unknown Command");
        Serial.println("Use: L R A D Q E S P");
        break;
    }

    while (Serial.available()) {
      Serial.read();
    }
  }
}


#include <ESP32Servo.h>

Servo servo;

// ===== SERVO CALIBRATION (MICROSECONDS) =====
const int STOP_US  = 1520;
const int LEFT_US  = 1450;
const int RIGHT_US = 1590;

// ===== STEP CALIBRATION (YOUR REAL VALUES) =====
// 90° = 18 steps (measured)
const int STEPS_90  = 18;
const int STEPS_180 = 36;
const int STEPS_360 = 72;

// ===== TIMING =====
const int STEP_PULSE_MS = 15; // calibration area 20  - 40 rigth now 30ish

const int STEP_PAUSE_MS = 80;

// ===== COUNTERS =====
long leftSteps = 0;
long rightSteps = 0;
long position = 0;

void setup() {
  Serial.begin(115200);

  servo.attach(26);
  servo.writeMicroseconds(STOP_US);

  Serial.println("=================================");
  Serial.println("CALIBRATED CONTINUOUS SERVO STEPPER");
  Serial.println("=================================");
  Serial.println("L = 90° left");
  Serial.println("R = 90° right");
  Serial.println("A = 180° left");
  Serial.println("D = 180° right");
  Serial.println("Q = 360° left");
  Serial.println("E = 360° right");
  Serial.println("P = print status");
  Serial.println("=================================");
}

void printStatus() {
  Serial.println();
  Serial.println("----- STATUS -----");

  Serial.print("Position: ");
  Serial.println(position);

  Serial.print("Left Steps: ");
  Serial.println(leftSteps);

  Serial.print("Right Steps: ");
  Serial.println(rightSteps);

  Serial.println("------------------");
}

void stepLeft() {
  servo.writeMicroseconds(LEFT_US);
  delay(STEP_PULSE_MS);

  servo.writeMicroseconds(STOP_US);
  delay(STEP_PAUSE_MS);

  leftSteps++;
  position--;
}

void stepRight() {
  servo.writeMicroseconds(RIGHT_US);
  delay(STEP_PULSE_MS);

  servo.writeMicroseconds(STOP_US);
  delay(STEP_PAUSE_MS);

  rightSteps++;
  position++;
}

void moveLeftSteps(int steps) {
  Serial.print("Moving LEFT ");
  Serial.print(steps);
  Serial.println(" steps");

  for (int i = 0; i < steps; i++) {
    stepLeft();
  }

  printStatus();
}

void moveRightSteps(int steps) {
  Serial.print("Moving RIGHT ");
  Serial.print(steps);
  Serial.println(" steps");

  for (int i = 0; i < steps; i++) {
    stepRight();
  }

  printStatus();
}

void loop() {

  if (Serial.available()) {

    char cmd = toupper(Serial.read());

    switch (cmd) {

      case 'L':
        moveLeftSteps(STEPS_90);
        break;

      case 'R':
        moveRightSteps(STEPS_90);
        break;

      case 'A':
        moveLeftSteps(STEPS_180);
        break;

      case 'D':
        moveRightSteps(STEPS_180);
        break;

      case 'Q':
        moveLeftSteps(STEPS_360);
        break;

      case 'E':
        moveRightSteps(STEPS_360);
        break;

      case 'P':
        printStatus();
        break;

      case '\n':
      case '\r':
        break;

      default:
        Serial.println("Use: L R A D Q E P");
        break;
    }

    while (Serial.available()) {
      Serial.read();
    }
  }
}
