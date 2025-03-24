// Receiver Channels (PWM Inputs)
#define CH1 14  // Forward-Backward
#define CH2 12  // Left-Right

// Motor Driver Pins
#define ENA 5   // Speed control for Motor A
#define IN1 18  // Motor A direction
#define IN2 19  

#define ENB 23  // Speed control for Motor B
#define IN3 21  // Motor B direction
#define IN4 22  

int ch1Value, ch2Value;

// Read PWM signal from receiver and map it to motor values
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup() {
  Serial.begin(115200);
  
  // for receiver
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  
  //  for motor controller
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void move(int speedA, int speedB, bool dirA, bool dirB) {
  analogWrite(ENA, abs(speedA));
  analogWrite(ENB, abs(speedB));

  digitalWrite(IN1, dirA);
  digitalWrite(IN2, !dirA);
  digitalWrite(IN3, dirB);
  digitalWrite(IN4, !dirB);
}

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);  // Forward-Backward
  ch2Value = readChannel(CH2, -100, 100, 0);  // Left-Right

  Serial.print("CH1 (Forward-Backward): ");
  Serial.print(ch1Value);
  Serial.print(" | CH2 (Left-Right): ");
  Serial.println(ch2Value);

  int speed = map(abs(ch1Value), 0, 100, 0, 255);  // Convert CH1 to PWM speed
  int turn = map(abs(ch2Value), 0, 100, 0, 255);   // Convert CH2 to PWM turn speed

  if (ch1Value > 10) {  // Move Forward
    move(speed, speed, HIGH, HIGH);
  } else if (ch1Value < -10) {  // Move Backward
    move(speed, speed, LOW, LOW);
  } else if (ch2Value > 10) {  // Turn Right
    move(turn, turn, HIGH, LOW);
  } else if (ch2Value < -10) {  // Turn Left
    move(turn, turn, LOW, HIGH);
  } else {  // Stop Motors
    move(0, 0, HIGH, HIGH);
  }

  delay(20);  // Small delay for stability
}
