#include <SwitecX25.h>

#include "Command.h"
#include "RotaryEncoder.h"
#include "IntRotaryEncoder.h"
#include "LCD.h"

/////////////////////////////////////
//  0  RX
//  1  TX
//  2       INT
//  3  PWM  INT
//  4
//  5  PWM
//  6  PWM
//  7
//
//  8
//  9  PWM
// 10  PWM
// 11  PWM
// 12
// 13
/////////////////////////////////////

Command cmd;

// 6 steps per rotation, so 60 degrees per step of motor
// 180:1 gear ration
// 1/3 degree per step of needle
// 315 degrees full-scale-deflection of needle

// limit to 232 degrees sweep for square thermo dials
// at 3 steps per degree is 696 steps
#define MOTOR_STEPS (696)
SwitecX25 motors[] =  {
  SwitecX25(MOTOR_STEPS, 8,9,12,13),
  SwitecX25(MOTOR_STEPS, 4, 5, 6, 7)
};
const unsigned int motorCount = sizeof(motors)/sizeof(*motors);

//RotaryEncoder encoder(2,3);
IntRotaryEncoder encoder;

LCD lcds[] = {LCD(10), LCD(11)};
const unsigned int lcdCount = sizeof(lcds)/sizeof(*lcds);

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
  motors[1].accelTable = accelTable;
  motors[1].maxVel = accelTable[3-1][0];
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
    int pos = motors[1].targetStep + delta;
    if (pos<0) pos=motors[1].steps-1;
    if (pos>=motors[1].steps) pos=0;
    motors[1].setPosition(pos);
  }
  motors[0].update();
  motors[1].update();
  if (cmd.parseInput()) {
    //cmd.dump();
    if (cmd.address[1]<motorCount) {
      SwitecX25 *motor = motors+cmd.address[1];
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
        case 'l':
          LCD *lcd = lcds + cmd.address[1];
          lcd->set(cmd.value[1]);
          break;
      }
    }
  }
}



