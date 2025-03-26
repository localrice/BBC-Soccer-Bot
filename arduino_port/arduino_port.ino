// Receiver channels
#define CH4 A2  // Left/Right
#define CH3 A1  // Brake when lowered
#define CH2 A0  // Forward/Backward

// Motor control pins
#define ENA A3   // Speed control for Motor A (Use PWM pin if needed)
#define IN1 11   // Motor A direction
#define IN2 12

#define ENB A4   // Speed control for Motor B (Use PWM pin if needed)
#define IN3 8   // Motor B direction
#define IN4 9

void setup(){
  Serial.begin(115200);

  pinMode(CH4, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// Read PWM signal and map to range with padding
int readPWMChannel(int channelInput, int minOutput, int maxOutput, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 40000); // Extended timeout
  if (ch < 100 || ch > 2500) return defaultValue; // Filter invalid signals
  int mappedValue = map(ch, 986, 1978, minOutput, maxOutput);
  return (abs(mappedValue) < 10) ? 0 : mappedValue;  // Apply dead zone padding of -10 to +10
}

void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void applyBrakes() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);  // Full brake
  analogWrite(ENB, 255);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop(){
  int ch2 = readPWMChannel(CH2, -100, 100, 0);
  int ch4 = readPWMChannel(CH4, -100, 100, 0);
  int ch3 = readPWMChannel(CH3, -100, 100, 0);

  if (ch3 < -80) { // Brake condition
    Serial.println("Applying brakes!");
    applyBrakes();
    return;  // Skip other controls while braking
  }

  // Priority: Left/Right first
  if(ch4 != 0) {
    if(ch4 > 0) {
      Serial.print("Turning left with speed: ");
      Serial.println(ch4);
      turnLeft(ch4 * 255.0 / 100.0);
    } else {
      Serial.print("Turning right with speed: ");
      Serial.println(-ch4);
      turnRight(-ch4 * 255.0 / 100.0);
    }
  } else if (ch2 != 0) { // Only move forward/backward if no turn input
    if(ch2 > 0) {
      Serial.print("Moving forward with speed: ");
      Serial.println(ch2);
      moveForward(ch2 * 255.0 / 100.0);
    } else {
      Serial.print("Moving backward with speed: ");
      Serial.println(-ch2);
      moveBackward(-ch2 * 255.0 / 100.0);
    }
  } else {
    stopMotors();
  }
}