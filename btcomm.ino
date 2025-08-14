#include <DC_driver.h>
#include "comm.h"

DC_driver motor_1(7, 8, 6);
DC_driver motor_2(4, 2, 5);
DC_driver motor_3(A1, A0, 10);
DC_driver motor_4(12, 11, 9);

Comm comm(&Serial);

void setup() 
{
  motor_1.begin();
  motor_2.begin();
  motor_3.begin();
  motor_4.begin();

  Serial.begin(9600);
}

void loop() 
{
  Message msg = comm.check_incoming();
  
  if (msg.type == SET_MOTORS) 
  {
    handle_set_motors(msg.set_motors);
  }
  else if (msg.type == TURN) 
  {
    handle_turn(msg.turn);
  }
  else if (msg.type == MOVE_LR)
  {
    handle_move_lr(msg.move_lr);
  }
}

void handle_set_motors(SetMotorsPayload payload) 
{
  motor_1.analogMove(payload.direction[0], payload.speed[0]);
  motor_2.analogMove(payload.direction[1], payload.speed[1]);
  motor_3.analogMove(payload.direction[2], payload.speed[2]);
  motor_4.analogMove(payload.direction[3], payload.speed[3]);

  // TODO(Richo): This is just for debugging...
  Serial.print("SET_MOTORS: [");
  for (int i = 0; i < 4; i++) 
  {
    if (i > 0) Serial.print(", ");
    Serial.print("(");
    if (payload.direction[i]) 
    {
      Serial.print("F");
    } 
    else 
    {
      Serial.print("B");
    }
    Serial.print(" ");
    Serial.print((int)payload.speed[i]);
    Serial.print(")");
  }
  Serial.println("]");
}

void handle_turn(TurnPayload payload)
{
  motor_1.analogMove(payload.is_clockwise, payload.speed);
  motor_2.analogMove(payload.is_clockwise, payload.speed);
  motor_3.analogMove(payload.is_clockwise, payload.speed);
  motor_4.analogMove(payload.is_clockwise, payload.speed);

  Serial.print("TURN: ");
  Serial.print(payload.is_clockwise);
  Serial.print(", ");
  Serial.println(payload.speed);
}


void handle_move_lr(MoveLRPayload payload)
{
  motor_1.analogMove(!payload.is_left, payload.speed);
  motor_2.analogMove(payload.is_left, payload.speed);
  motor_3.analogMove(payload.is_left, payload.speed);
  motor_4.analogMove(!payload.is_left, payload.speed);

  Serial.print("MOVE_LR: ");
  Serial.print(payload.is_left);
  Serial.print(", ");
  Serial.println(payload.speed);
}

