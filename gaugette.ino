#include <SwitecX25.h>

#include "Command.h"
#include "RotaryEncoder.h"
#include "IntRotaryEncoder.h"
#include "LED.h"
#include "Switch.h"

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
#define MOTOR_OFF_POS (MOTOR_STEPS/2)

SwitecX25 motors[] =  {
  SwitecX25(MOTOR_STEPS, 4,5,6,7),
  SwitecX25(MOTOR_STEPS, 8,9,11,12)
};
const unsigned int motorCount = sizeof(motors)/sizeof(*motors);

//RotaryEncoder encoder(2,3);
//IntRotaryEncoder encoder;

#define LED_BRIGHT  100
#define LED_DIM      20
#define LED_OFF       0
#define LED_PERIOD 1000

LED leds[] = {LED(3), LED(10)};
const unsigned int ledCount = sizeof(leds)/sizeof(*leds);

#define MOTOR_INACTIVE_TIMEOUT 5000
unsigned long motorUpdate[] = {0,0};
unsigned char motorActive[] = {false, false};

Switch switch1(A1);
Switch switch2(A0);
boolean active = true;

void setLedBrightness(int index)
{
  unsigned char brightness = LED_OFF;
  if (switch2.set) {
    brightness = LED_OFF;
    Serial.println("off - switch2 on");
  } else if (!switch1.set) {
    brightness = LED_OFF;
    Serial.println("off - switch1 off");
  } else if (motorActive[index]) {
    brightness = LED_BRIGHT;
    Serial.println("bright - active");
  } else {
    brightness = LED_DIM;
    Serial.println("dim - inactive");
  }
  leds[index].set(brightness);
}

void setup(void) {
  Serial.begin(9600);
  Serial.print(motorCount);
  Serial.print(" motors. ");    
  Serial.println("Go!");
  
  // excercise the code to override the acceleration table.
  // static unsigned short accelTable[][2] = {
  //   {   20, 3000},
  //   {  100, 1000},
  //   {  300,  600}
  //};
  //motors[1].accelTable = accelTable;
  //motors[1].maxVel = accelTable[3-1][0];

  leds[0].period = LED_PERIOD;  // milliseconds to change
  leds[1].period = LED_PERIOD;

}

void loop(void) {
  /*
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
  */
  
  // switch1 on turns on light
  if (switch1.changed()) {
    Serial.print("switch1 ");
    Serial.println(switch1.set ? "on" : "off");
    setLedBrightness(0);
    setLedBrightness(1);
  }
  
  // switch2 on turns everything off
  if (switch2.changed()) {
    Serial.print("switch2 ");
    Serial.println(switch2.set ? "on" : "off");
    active = !switch2.set;
    if (switch2.set) {
      // inactive
      motors[0].zero();
      motors[1].zero();
      motors[0].setPosition(MOTOR_OFF_POS);
      motors[1].setPosition(MOTOR_OFF_POS);
    } else {
    }    
    setLedBrightness(0);
    setLedBrightness(1);
  }
  
  // if motors become inactive, dim leds
  {
    for (int i=0;i<motorCount;i++) {
      if (motorActive[i]) {
        if ((millis() - motorUpdate[i]) > MOTOR_INACTIVE_TIMEOUT) {
          motorActive[i] = false;
          setLedBrightness(i);
          Serial.println("inactive");
        }
      }
    }
  }
  
  motors[0].update();
  motors[1].update();
  leds[0].update();
  leds[1].update();
  if (cmd.parseInput()) {
    int index = cmd.address[1];
    //cmd.dump();
    if (index<motorCount) {
      SwitecX25 *motor = motors+index;
      switch (cmd.command) {
        case 'z':
          motor->zero(); 
          break;
        case 's':
          if (active) motor->setPosition(cmd.value[1]);
          motorActive[index] = true;
          motorUpdate[index] = millis();
          setLedBrightness(index);
          break;
        case 'r':
          // r <n> <steps> set motor range
          motor->steps = cmd.value[1];
          break;
        case 'l':
        {
          LED *led = leds + cmd.address[1];
          led->set(cmd.value[1]);
          break;
        }
        case 'x':
          motor->steps = 2000; // > 3 * 360
          for (int i=0;i<cmd.value[1];i++) {
            motor->currentStep = 0;    
            motor->setPosition(360*3);
            //while (motor->currentStep < 360*3) motor->update();
            while (!motor->stopped) motor->update();
          }
          motor->currentStep = 0;
          motor->setPosition(0);
          break;
      }
    }
  }
}



