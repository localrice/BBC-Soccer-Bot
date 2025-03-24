#define CH1 14
#define CH2 12

int ch1Value, ch2Value;

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup() {
  Serial.begin(115200);

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);

}

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);


  Serial.println("Channel 1: ");
  Serial.println(ch1Value);

  Serial.println("Channel 2: ");
  Serial.println(ch2Value);
  
}