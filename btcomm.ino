//ejemplo para crear animaciones en módulo matriz LED con Arduino basado en MAX7219
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 4

#define CLK_PIN   A4
#define DATA_PIN  A2
#define CS_PIN    A3

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);


struct Stack {
    uint8_t items[32];
    uint8_t capacity = 32;
    uint8_t top = 0;

    void init() {
      for (int i = 0; i < capacity; i++) {
        items[i] = 0;
      }
    }

    void push(uint8_t value) {
        items[top] = value;
        top++;
        while (top >= capacity) top -= capacity;
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

Stack s;

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

  s.init();

  Serial.begin(9600);
}

uint8_t counter = 0;

void loop() {
  s.push(counter++);
  s.display_on(&mx);
  delay(10);
}
