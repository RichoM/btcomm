#pragma once
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 4

#define CLK_PIN   A4
#define DATA_PIN  A2
#define CS_PIN    A3


static MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);


void debug_begin() {
  // inicializar el objeto mx
  mx.begin();

  // poniendo la matriz en modo de prueba
  mx.control(MD_MAX72XX::TEST, true);
  delay(2000);

  // desactivando el modo de prueba
  mx.control(MD_MAX72XX::TEST, false);

  // Establecer intencidad a un valor de 5
  mx.control(MD_MAX72XX::INTENSITY, 0);
}

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

struct Stack {
    uint8_t items[24];
    uint8_t capacity = 24;
    uint8_t top = 0;

    Stack() {
      for (int i = 0; i < capacity; i++) {
        items[i] = 0;
      }
    }

    void push(uint8_t value) {
        items[top] = value;
        top++;
        while (top >= capacity) top -= capacity;

        
      this->display_on(&mx);
    }

    void display_on(MD_MAX72XX* mx) {
        int idx = top;
        int count = 0;
        while (count < capacity) {
            idx--;
            while (idx < 0) idx += (int)capacity;
            mx->setRow((uint8_t)(count / 8), count % 8, items[idx]);
            count++;
        }
    }
};

static Stack s;
