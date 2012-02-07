#include <SwitecX25.h>

#include "Command.h"
#include "RotaryEncoder.h"
#include "IntRotaryEncoder.h"

Command cmd;

// 6 steps per rotation, so 60 degrees per step of motor
// 180:1 gear ration
// 1/3 degree per step of needle
// 315 degrees full-scale-deflection of needle

// limit to 232 degrees sweep for square thermo dials
// at 3 steps per degree is 696 steps
#define MOTOR_STEPS (696)
SwitecX25 motor1(MOTOR_STEPS, 8, 9, 10, 11);
SwitecX25 motor2(MOTOR_STEPS, 4, 5, 6, 7);
SwitecX25 *motors[] = {&motor1,&motor2};
const unsigned int motorCount = sizeof(motors)/sizeof(*motors);

//RotaryEncoder encoder(2,3);
IntRotaryEncoder encoder;

void setup(void) {
  Serial.begin(9600);
  Serial.print(motorCount);
  Serial.print(" motors. ");    
  Serial.println("Go!");
  
  // excercise the code to override the acceleration table.
  static unsigned short accelTable[][2] = {
    {   20, 3000},
    {  100, 1000},
    {  300,  600}
  };
  motor2.accelTable = accelTable;
  motor2.maxVel = accelTable[3-1][0];
}

void loop(void) {
  {
    static int n = 0;
    int delta = encoder.read();
    if (delta!=0) {
      n += delta;
      Serial.print(delta);
      Serial.print(" ");
      Serial.println(n);
    }
    int pos = motor2.targetStep + delta;
    if (pos<0) pos=motor2.steps-1;
    if (pos>=motor2.steps) pos=0;
    motor2.setPosition(pos);
  }
  motor1.update();
  motor2.update();
  if (cmd.parseInput()) {
    //cmd.dump();
    if (cmd.address[1]<motorCount) {
      SwitecX25 *motor = motors[cmd.address[1]];
      switch (cmd.command) {
        case 'z':
          motor->zero();        
          break;
        case 's':
          motor->setPosition(cmd.value[1]);
          break;
        case 'r':
          // r <n> <steps> set motor range
          motor->steps = cmd.value[1];
          break;
      }
    }
  }
}



