// Receiver channels
#define CH4 14  // Left/Right
#define CH2 12  // Forward/Backward
#define CH3 27  // Brake when lowered

// Motor control pins
#define ENA 17   // Speed control for Motor A
#define IN1 5    // Motor A direction
#define IN2 18

#define ENB 23   // Speed control for Motor B
#define IN3 19   // Motor B direction
#define IN4 22   

void setup() {
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

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Move Forward
void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Move Backward
void moveBackward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Turn Right
void turnRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Turn Left
void turnLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Stop Motors (No Brake)
void stopMotors() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

// Brake Motors (Shorts both terminals)
void brakeMotors() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void loop() {
  // Read channels
  int ch4Value = readChannel(CH2, -100, 100, 0); // Left/Right
  int ch2Value = readChannel(CH4, -100, 100, 0); // Forward/Backward
  int ch3Value = readChannel(CH3, -100, 100, 0); // Brake switch

  Serial.print("CH4 (left/right): "); Serial.println(ch4Value);
  Serial.print("CH2 (up/down): "); Serial.println(ch2Value);
  Serial.print("CH3 (brake switch): "); Serial.println(ch3Value);

  int speed = map(abs(ch2Value), 0, 100, 0, 255);  
  int turn = map(abs(ch4Value), 0, 100, 0, 255);   

  // If CH3 is lowered to -50 or less, brake instead of stopping
  if (ch3Value <= -50) {
    brakeMotors();
  } 
  else {
    if (ch2Value > 10) moveForward(speed);
    else if (ch2Value < -10) moveBackward(speed);
    else if (ch4Value > 10) turnRight(turn);
    else if (ch4Value < -10) turnLeft(turn);
    else stopMotors(); // Normal stop
  }

  delay(100);
}
