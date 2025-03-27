// Receiver channels
#define CH1 A2  // Left/Right
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

//----------------------------
#define sensitivity 0.5f

void setup(){
  Serial.begin(115200);

  pinMode(CH1, INPUT);
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
  return (abs(mappedValue) < 4) ? 0 : mappedValue;  // Apply dead zone padding of -10 to +10
}

void motorA(int speed){
  if(speed >0 ){
    analogWrite(ENA, speed*2.55);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }else{
    analogWrite(ENA, -speed*2.55);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
}

void motorB(int speed){
  if(speed >0 ){
    analogWrite(ENB, speed*2.55);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }else{
    analogWrite(ENB, -speed*2.55);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
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
  delay(1000);


  Serial.println("200");
  
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
//#define testing
void loop(){
  //----------------------------------------------------------------------
  #ifdef debug
  int ch1 = readPWMChannel(CH1, -100, 100, 0);
  
  Serial.println("Debuging\n");
  Serial.println(ch1);
  moveForward(255);
  
  #endif

  //---------------------------------------------------------------------------
  #ifndef debug

  int ch3 = readPWMChannel(CH3, 0, 100, 0);
  if (ch3 == 0) { // Brake condition
    Serial.println("Applying brakes!");
    applyBrakes();
    return;  // Skip other controls while braking
  }
  
  int speedA =0;
  int speedB =0;

  // Priority: Left/Right first
  //proper priority
  int ch1 = readPWMChannel(CH1, -100, 100, 0);
  int ch2 = readPWMChannel(CH2, -100, 100, 0);
  
  digitalWrite(REYE, HIGH);
  digitalWrite(LEYE, HIGH);

  //added nesting for proper PRIORITY (I am looking at you Jaikhlong)
  if(ch1 > 0) {
    digitalWrite(LEYE, LOW);
    //turnRight(2.55*ch3); can't do mixing with this approach
    speedA -= ch3;
    speedB += ch3;
    
    if(ch2 > 0){
      speedA += sensitivity*ch2;
    }else if(ch2 < 0){
      speedB -= sensitivity*ch2;
    }
    
    motorA(speedA);
    motorB(speedB);
    return;
  }
  else if(ch1 < 0){
    digitalWrite(REYE, LOW);
    //turnLeft(2.55*ch3); can't do mixing & 3rd channel sensitivity, for jack sht, with the dummified procedural style (ohh please reffer to the 69 functions for doing specific task, hell naw)
    speedA += ch3;
    speedB -= ch3;
    
    if(ch2 > 0){
      speedB += sensitivity*ch2;
    }else if(ch2 < 0){
      speedA -= sensitivity*ch2;
    }
    motorA(speedA);
    motorB(speedB);
    return;
  }
  if(ch2 > 0){
    speedA += ch3;
    speedB += ch3;
  }else if(ch2 < 0){
    speedA -= ch3;
    speedB -= ch3;
  }
  motorA(speedA);
  motorB(speedB);
  
  #ifdef testing
  Serial.print("blah blah blah, bluh bluh bluh ");
  Serial.println(speedA);
  Serial.println(speedB);
  #endif
  
  if(ch2 == 0 && ch1 == 0) {
    stopMotors();
  }

  #endif 
 
}