// Receiver channels
#define CH4 A2  // Left/Right
#define CH3 A1  // Brake when lowered
#define CH2 A0  // Forward/Backward

//polarity control could be abstracted away: OUT1 OUT2 OUT3 OUT4
//in moveForward, backward, etc.

// Motor control pins
#define ENA A3   // Speed control for Motor A
#define OUT1 11    // Motor A direction
#define OUT2 12

#define ENB A4  // Speed control for Motor B
#define OUT3 8    // Motor B direction
#define OUT4 9   

void setup() {
  Serial.begin(115200);

  //could be put away in a constructor
  pinMode(CH4, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
}

//some identifiers renamed
int readPWMChannel(int channelInput, int minOutput, int maxOutput, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 989, 1978, minOutput, maxOutput);
}

void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, LOW);
  digitalWrite(OUT3, HIGH);
  digitalWrite(OUT4, LOW);
}

void moveBackward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, HIGH);
  digitalWrite(OUT3, LOW);
  digitalWrite(OUT4, HIGH);
}

void turnLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(OUT1, LOW);  
  digitalWrite(OUT2, HIGH);  // Reverse left motor
  digitalWrite(OUT3, HIGH);
  digitalWrite(OUT4, LOW);   // Forward right motor
}

void turnRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(OUT1, HIGH); 
  digitalWrite(OUT2, LOW);   // Forward left motor
  digitalWrite(OUT3, LOW);
  digitalWrite(OUT4, HIGH);  // Reverse right motor
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


void loop() {
  moveForward(150);
  delay(2000);
  stopMotors();
  delay(2000);
  
  // Read channels
  int ch4Value = readPWMChannel(CH2, -100, 100, 0); // Left/Right
  int ch2Value = readPWMChannel(CH4, -100, 100, 0); // Forward/Backward
  int ch3Value = readPWMChannel(CH3, -100, 100, 0); // Brake switch

  Serial.print("CH4 (left/right): "); Serial.println(ch4Value);
  Serial.print("CH2 (up/down): "); Serial.println(ch2Value);
  Serial.print("CH3 (brake switch): "); Serial.println(ch3Value);

  int speed = map(abs(ch2Value), 0, 100, 0, 255);  
  int turn = map(abs(ch4Value), 0, 100, 0, 255);   

  // If CH3 is lowered to -50 or less, brake instead of stopping
  if (ch3Value <= -50) {
    //brakeMotors();
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