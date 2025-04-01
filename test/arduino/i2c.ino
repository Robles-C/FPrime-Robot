/*#include <Wire.h>

// Motor pins
const int LEFT_MOTOR_PWM  = 5;
const int LEFT_MOTOR_DIR  = 4;
const int RIGHT_MOTOR_PWM = 6;
const int RIGHT_MOTOR_DIR = 7;

void setup() {
  // Setup I2C 
  Wire.begin(0x08); 
  Wire.onReceive(receiveEvent);

  // Setup motor pins
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);
}

void loop() {
  // I²C will trigger receiveEvent()
}

void receiveEvent(int numBytes) {
  if (numBytes < 2) return;

  byte high = Wire.read();
  byte low  = Wire.read();

  uint16_t packet = (high << 8) | low;
  char direction = (packet >> 8) & 0xFF;
  byte speed = packet & 0xFF;

  speed = constrain(speed, 0, 255);

  handleMotorCommand(direction, speed);
}

void handleMotorCommand(char dir, byte speed) {
  Serial.print("Command: ");
  Serial.print(dir);
  Serial.print(" Speed: ");
  Serial.println(speed);

  switch (dir) {
    case 'F': // Forward
      digitalWrite(LEFT_MOTOR_DIR, HIGH);
      digitalWrite(RIGHT_MOTOR_DIR, HIGH);
      analogWrite(LEFT_MOTOR_PWM, speed);
      analogWrite(RIGHT_MOTOR_PWM, speed);
      break;

    case 'B': // Backward
      digitalWrite(LEFT_MOTOR_DIR, LOW);
      digitalWrite(RIGHT_MOTOR_DIR, LOW);
      analogWrite(LEFT_MOTOR_PWM, speed);
      analogWrite(RIGHT_MOTOR_PWM, speed);
      break;

    case 'L': // Turn left
      digitalWrite(LEFT_MOTOR_DIR, LOW);
      digitalWrite(RIGHT_MOTOR_DIR, HIGH);
      analogWrite(LEFT_MOTOR_PWM, speed);
      analogWrite(RIGHT_MOTOR_PWM, speed);
      break;

    case 'R': // Turn right
      digitalWrite(LEFT_MOTOR_DIR, HIGH);
      digitalWrite(RIGHT_MOTOR_DIR, LOW);
      analogWrite(LEFT_MOTOR_PWM, speed);
      analogWrite(RIGHT_MOTOR_PWM, speed);
      break;

    case 'S': // Stop
    default:
      analogWrite(LEFT_MOTOR_PWM, 0);
      analogWrite(RIGHT_MOTOR_PWM, 0);
      break;
  }
}*/
