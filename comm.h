#pragma once
#include "Arduino.h"
#include "types.h"
#include "reader.h"

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

public:
  Comm(Stream* serial) : reader(serial) 
  {
    this->serial = serial;
  }
  
  Message check_incoming() 
  {
    if (!reader.available()) return NO_MESSAGE;
    
    bool timeout;
    uint8 in = reader.next(timeout);
    
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
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_1 = reader.next(timeout);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_2 = reader.next(timeout);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_3 = reader.next(timeout);
    if (timeout) return NO_MESSAGE;
    
    uint8 motor_4 = reader.next(timeout);
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