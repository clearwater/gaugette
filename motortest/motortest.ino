int motorPin1 = 8;   // orange
int motorPin2 = 9;   // brown
int motorPin3 = 10;  // blue
int motorPin4 = 11;  // green
int delayTime = 5;
int count = 157;

// 6 steps per motor rotation
// 180:1 gear
// 180 * 6 steps per rotation
// 2 degrees per 6-states
// stops liit to 315 degrees
// 315 = 157.5 steps
// maximum speed seems to be about 800 microseconds delay

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  center();
}

void pause()
{
  delayMicroseconds(800);
}

void state1() {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    pause();
}

void state2() {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
    pause();
}

void state3() {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    pause();
}

void state4() 
{
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    pause();
}

void state5() {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, HIGH);
    pause();
}

void state6() 
{
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    pause();
}


void center()
{
  for (int i=0;i<count;i++) {
    state6();
    state5();
    state4();
    state3();
    state2();
    state1();
  }

  for (int i=0;i<count;i++) {
    state1();
    state2();
    state3();
    state4();
    state5();
    state6();
  }
  
  for (int i=0;i<count/2;i++) {
    state6();
    state5();
    state4();
    state3();
    state2();
    state1();
  }
  
}

void loop()
{
}
