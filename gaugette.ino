

#import "Command.h"
#import "SwitecX25.h"

Command cmd;

// 6 steps per rotation, so 60 degrees per step
// 180:1 gear ration
// 1/3 degree per step
// 315 degrees full-scale-deflection

SwitecX25 motor1(315 * 3, 8, 9, 10, 11);
SwitecX25 motor2(315 * 3, 4,5,6,7);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Gaugette!");
  motor1.zero();  
  motor2.zero();  
}

void loop(void) {
  motor1.update();
  motor2.update();
  if (cmd.parseInput()) {
    //cmd.dump();
    SwitecX25 *motor = (cmd.address[1]==0) ? &motor1 : &motor2;
    switch (cmd.command) {
      case 'z':
        motor->zero();
        break;
      case 's':
        motor->setPosition(cmd.value[1]);
        break;
      case 'a':
        //motor->setAccel(cmd.value[1],cmd.value[2]);
        break;
      case 'v':
        //motor->setSpeed(cmd.value[1],cmd.value[2]);
        break;
      case 'd':
        //motor->setDelay(cmd.value[1],cmd.value[2]);
        break;
    }
  }
}



