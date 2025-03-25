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

//some identifiers renamed
int readPWMChannel(int channelInput, int minOutput, int maxOutput, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 986, 1978, minOutput, maxOutput);
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
  digitalWrite(IN2, HIGH);  // Reverse left motor
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);   // Forward right motor
}

void turnRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);   // Forward left motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Reverse right motor
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop(){

  int ch2 = readPWMChannel(CH2, -100, 100, 0);
  if(ch2 != 0){
    if(ch2>0){
      //move forward
      Serial.print("Moving forward with speed");
      Serial.print(ch2); Serial.println();
      moveForward(ch2*225.0/100.0);
    }
    else{
      //move backward
      Serial.print("Moving backward with speed");
      Serial.print(ch2); Serial.println();
      moveBackward(-ch2*225.0/100.0);
      
    }
  }

  int ch4 = readPWMChannel(CH4, -100, 100, 0);
  if(ch4 != 0){
    if(ch4>0){
      //move forward
      Serial.print("Moving forward with speed");
      Serial.print(ch4); Serial.println();
      moveForward(ch4*225.0/100.0);
    }
    else{
      //move backward
      Serial.print("Moving backward with speed");
      Serial.print(ch4); Serial.println();
      moveBackward(-ch4*225.0/100.0);
      
    }
  }

}

//1978 : max
//989 : min