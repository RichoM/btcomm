#include <DC_driver.h>
#include "comm.h"

DC_driver motor_1(7, 8, 6);
DC_driver motor_2(4, 2, 5);
DC_driver motor_3(A1, A0, 10);
DC_driver motor_4(12, 11, 9);

Comm comm(&Serial);

void setup() {
  motor_1.begin();
  motor_2.begin();
  motor_3.begin();
  motor_4.begin();

  Serial.begin(9600);
}

void loop() {
  Message msg = comm.check_incoming();
  if (msg.type == SET_MOTORS) {
    handle_set_motors(msg.set_motors);
  }
}

void handle_set_motors(SetMotorsPayload payload) {
  motor_1.analogMove(payload.direction[0], payload.speed[0]);
  motor_2.analogMove(payload.direction[1], payload.speed[1]);
  motor_3.analogMove(payload.direction[2], payload.speed[2]);
  motor_4.analogMove(payload.direction[3], payload.speed[3]);

  // TODO(Richo): This is just for debugging...
  Serial.print("SET_MOTORS: [");
  for (int i = 0; i < 4; i++) {
    if (i > 0) Serial.print(", ");
    Serial.print((int)payload.speed[i]);
  }
  Serial.println("]");
}
