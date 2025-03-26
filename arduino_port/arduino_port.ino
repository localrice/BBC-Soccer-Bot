// Receiver channels
#define CH4 A2  // Left/Right
#define CH3 A1  // Brake when lowered
#define CH2 A0  // Forward/Backward

//LED eye pins
#define LEYE A4
#define REYE A5

// Motor control pins
#define ENA 6   // Speed control for Motor A (Use PWM pin if needed)
#define IN1 11   // Motor A direction
#define IN2 12

#define ENB 5   // Speed control for Motor B (Use PWM pin if needed)
#define IN3 8   // Motor B direction
#define IN4 9

void setup(){
  Serial.begin(115200);

  pinMode(CH4, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);

  pinMode(LEYE, OUTPUT);
  pinMode(REYE, OUTPUT);
  digitalWrite(LEYE, HIGH);
  digitalWrite(REYE, HIGH);

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

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void moveBackward(int speed) {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

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

void test_motors(){

  Serial.println("1");
  moveForward(50);
  delay(1000);


  Serial.println("200");
  moveBackward(255);
  
  delay(1000);
  Serial.println("breeh");
  Serial.println("breeh");
  Serial.println("breeh");
  /*
  turnLeft(200);
  delay(500);
  turnRight(200);
  delay(500);
  */
}

//#define debug
#define testing
void loop(){
  int ch3 = readPWMChannel(CH3, 0, 100, 0);

  //---------------------------------------------------------------------------
  #ifndef debug
  if (ch3 == 0) { // Brake condition
    Serial.println("Applying brakes!");
    applyBrakes();
    return;  // Skip other controls while braking
  }

  // Priority: Left/Right first
  //proper priority
  int ch4 = readPWMChannel(CH4, -100, 100, 0);
  digitalWrite(LEYE, HIGH);
  if(ch4 > 0) {
    digitalWrite(LEYE, LOW);
    #ifdef testing
    Serial.print("Turning right with speed: ");
    Serial.println(ch4);
    #endif
    turnRight(ch4*(2.55*ch3/100));
  }  
  digitalWrite(REYE, HIGH);
  if(ch4 < 0){
    digitalWrite(REYE, LOW);
    #ifdef testing
    Serial.print("Turning left with speed: ");
    Serial.println(ch4*(2.55*ch3/100));
    #endif
    ch4 *=-1;
    turnLeft(ch4*2.55);
  }

  int ch2 = readPWMChannel(CH2, -100, 100, 0);
  if(ch2 > 0){
    moveForward(ch2*(2.55*ch3/100));
    #ifdef testing
    Serial.print("Moving forward with speed: ");
    Serial.println(-ch2);
    #endif
  }
  if(ch2 < 0) {
    ch2 *= -1;
    moveBackward(ch2*(2.55*ch3/100));
    #ifdef testing
    Serial.print("Moving backward with speed: ");
    Serial.println(ch2);
    #endif
  }
  if(ch2 == 0 && ch4 == 0) stopMotors();

  #endif 
 
  //----------------------------------------------------------------------
  #ifdef debug
  int ch4 = readPWMChannel(CH4, -100, 100, 0);
  
  Serial.println("Debuging\n");
  Serial.println(ch4);
  moveForward(255);
  
  #endif
}