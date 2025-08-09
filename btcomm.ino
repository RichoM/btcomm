
#include <SoftwareSerial.h>
// #include <DC_driver.h>
#include "debug.h"
#include "comm.h"

SoftwareSerial BTserial(8, 9); // RX | TX

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);


void display_mag(int idx, int value) {
  uint8_t v = round((float)constrain(value, 0, 255) / 255.0 * 8);
  uint8_t chr = 0;
  switch (v) {
    case 0: chr =   0; break;
    case 1: chr =   1; break;
    case 2: chr =   3; break;
    case 3: chr =   7; break;
    case 4: chr =  15; break;
    case 5: chr =  31; break;
    case 6: chr =  63; break;
    case 7: chr = 127; break;
    case 8: chr = 255; break;
  }
  mx.setRow(3, 7 - idx, chr);
}

int motors = 0;
struct DC_driver {
  int id = 0;
  DC_driver(int en, int f, int b) {}
  void begin() {
    id = motors++;
  }
  void analogMove(bool direction, byte speed) {
    display_mag(id, speed);
  }
};

Stack s;
uint8_t counter = 0;

DC_driver motor_1(7, 8, 6);
DC_driver motor_2(4, 2, 5);
DC_driver motor_3(A1, A0, 10);
DC_driver motor_4(12, 11, 9);


Comm comm(&BTserial, &s);

void setup() {
  // inicializar el objeto mx
  mx.begin();

  // poniendo la matriz en modo de prueba
  mx.control(MD_MAX72XX::TEST, true);
  delay(2000);

  // desactivando el modo de prueba
  mx.control(MD_MAX72XX::TEST, false);

  // Establecer intencidad a un valor de 5
  mx.control(MD_MAX72XX::INTENSITY, 0);

  motor_1.begin();
  motor_2.begin();
  motor_3.begin();
  motor_4.begin();

  BTserial.begin(9600);
}

void loop() {
  Message msg = comm.check_incoming();
  if (msg.type == SET_MOTORS) {
    handle_set_motors(msg.set_motors);
  }
  s.display_on(&mx);

  // int pin = readNext();
  // int value = readNext();
  
  // for (int i = 0; i < 8; i++) {
  //   display_mag(i, (sin(((float)counter + i)*0.75) * 0.5 + 0.5) * 256);
  // }

  // motor_1.analogMove(true, 0);
  // motor_2.analogMove(true, 64);
  // motor_3.analogMove(true, 128);
  // motor_4.analogMove(true, 255);

  // s.push(counter++);
  // s.display_on(&mx);
  // delay(1);
}

void handle_set_motors(SetMotorsPayload payload) {
  motor_1.analogMove(payload.direction[0], payload.speed[0]);
  motor_2.analogMove(payload.direction[1], payload.speed[1]);
  motor_3.analogMove(payload.direction[2], payload.speed[2]);
  motor_4.analogMove(payload.direction[3], payload.speed[3]);
}
