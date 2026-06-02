# Continuous Servo Step Controller (ESP32)

Control a continuous rotation servo like a stepper motor using an ESP32, with serial commands for precise angle movements.

---

## Hardware

- ESP32
- Continuous rotation servo (signal wire → GPIO 26)
- Power supply

---

## Servo Calibration

This project uses two calibration methods depending on your servo:

### Method 1 — `servo.write()` (degrees)

| State | Value | Behavior |
|-------|-------|----------|
| Stop (neutral band) | 92–96 | No movement |
| STOP | 94 | Centered stop |
| LEFT | 90 | Slow left |
| RIGHT | 98 | Slow right |

> The neutral band on this servo is 92–96. Values outside this range cause movement; the further from center, the faster the spin.

### Method 2 — `servo.writeMicroseconds()` (µs)

| State | Value |
|-------|-------|
| STOP | 1520 µs |
| LEFT | 1450 µs |
| RIGHT | 1590 µs |

> Microsecond control gives finer resolution. Prefer this method if your servo is twitchy with degree values.

---

## Step Calibration

Steps are timed pulses — not encoder feedback. Accuracy depends on consistent voltage and pulse timing.

| Angle | Steps |
|-------|-------|
| 90°   | 18–20 |
| 180°  | 36–40 |
| 360°  | 72–80 |

Tune `STEP_PULSE_MS` (default: 15–20ms) and `STEP_PAUSE_MS` (default: 80–100ms) to match your servo's actual rotation.

---

## Serial Commands

Open Serial Monitor at **115200 baud**.

| Key | Action |
|-----|--------|
| `L` | Left 90° |
| `R` | Right 90° |
| `A` | Left 180° |
| `D` | Right 180° |
| `Q` | Left 360° |
| `E` | Right 360° |
| `S` | Stop |
| `P` | Print status |

---

## Known Limitations

- **No position feedback** — this is open-loop control. Errors accumulate over time.
- Small deviations in battery voltage affect speed and step accuracy.
- The gap between neutral band edge (92) and full stop (91) is only 1 unit — approach slowly when calibrating to avoid unexpected spin.

---

## Files

| File | Description |
|------|-------------|
| `servo_degree.ino` | Degree-based control using `servo.write()` |
| `servo_microseconds.ino` | Microsecond-based control using `servo.writeMicroseconds()` |

---

## Dependencies

- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) — install via Arduino Library Manager

code current
```
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

##
```


```
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

```

code working on the 2 variables I am focusing is the 

// ciritcal 1
const int STOP_US  = 1520;
const int LEFT_US  = 1450;
const int RIGHT_US = 1590;

and


critically its this 

// ===== TIMING =====
const int STEP_PULSE_MS = 15; // calibration area 20  - 40 rigth now 30ish

const int STEP_PAUSE_MS = 80;

outcome of this


// 90° = 18 steps (measured)
const int STEPS_90  = 18;
const int STEPS_180 = 36;
const int STEPS_360 = 72;
