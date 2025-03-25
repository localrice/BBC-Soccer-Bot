// Receiver channels
#define CH4 14  // Left/Right
#define CH2 12  // Forward/Backward
#define CH3 27  // Brake when lowered

// Motor control pins
#define ENA 17   // Speed control for Motor A
#define OUT1 5    // Motor A direction
#define OUT2 18

#define ENB 23   // Speed control for Motor B
#define OUT3 19   // Motor B direction
#define OUT4 22   

struct motor{
    void pin_init();
};

struct bot{
    motor M1, M2;
    //io handling
    void listener();

    void init();
};

void bot::pin_init(){
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

//this can be reduced
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
    int ch = pulseIn(channelInput, HIGH, 30000);
    if (ch < 100) return defaultValue;
    return map(ch, 1000, 2000, minLimit, maxLimit);
}

int read(int chno, int min, int max, int def){

    return map(
        pulseIn(chno, HIGH, 30000),
        1000,
        2000,
        min,
        max);
}
  
