#pragma once
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 4

#define CLK_PIN   A4
#define DATA_PIN  A2
#define CS_PIN    A3

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
