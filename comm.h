#pragma once
#include "Arduino.h"
#include "types.h"
#include "reader.h"
#include "debug.h"

struct SetMotorsPayload 
{
  bool direction[4];
  uint8 speed[4];
};

enum MessageType {
  SET_MOTORS = 1,
};

struct Message {
    MessageType type;
    union {
        SetMotorsPayload set_motors;
    };

    Message(MessageType type) {
        this->type = type;
    }
};

const Message NO_MESSAGE(0);

class Comm 
{
private:
  Stream* serial;
	StreamReader reader;

  Stack* stack;

public:
  Comm(Stream* serial, Stack* stack) : reader(serial) 
  {
    this->serial = serial;
    this->stack = stack;
  }
  
  Message check_incoming() 
  {
    if (!reader.available()) return NO_MESSAGE;
    
    bool timeout;
    uint8 in = reader.next(timeout);
    stack->push(in);
    
    if (timeout) return NO_MESSAGE;

    if (in == SET_MOTORS)
    {
      return read_set_motor_speeds();
    }
  }

  Message read_set_motor_speeds()
  {
    Message msg(SET_MOTORS);

    bool timeout;
    uint8 directions = reader.next(timeout);
    stack->push(directions);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_1 = reader.next(timeout);
    stack->push(motor_1);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_2 = reader.next(timeout);
    stack->push(motor_2);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_3 = reader.next(timeout);
    stack->push(motor_3);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_4 = reader.next(timeout);
    stack->push(motor_4);
    if (timeout) return NO_MESSAGE;
    
    SetMotorsPayload payload;
    payload.direction[0] = directions & 1;
    payload.direction[1] = directions & 2;
    payload.direction[2] = directions & 4;
    payload.direction[3] = directions & 8;
    payload.speed[0] = motor_1;
    payload.speed[1] = motor_2;
    payload.speed[2] = motor_3;
    payload.speed[3] = motor_4;

    msg.set_motors = payload;
    return msg;
  }
};