#pragma once
#include "Arduino.h"
#include "types.h"
#include "reader.h"

struct SetMotorsPayload 
{
  bool direction[4];
  uint8 speed[4];
};

struct TurnPayload
{
  bool is_clockwise;
  uint8 speed;
};

struct MoveLRPayload
{
  bool is_left;
  uint8 speed;
};

enum MessageType {
  SET_MOTORS = 1,
  TURN = 2,
  MOVE_LR = 3,
};

struct Message {
    MessageType type;
    union 
    {
        SetMotorsPayload set_motors;
        TurnPayload turn;
        MoveLRPayload move_lr;
    };

    Message(MessageType type) 
    {
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
    else if (in == TURN)
    {
      return read_turn();
    }
    else if (in == MOVE_LR)
    {
      return read_move_lr();
    }
  }

  Message read_set_motor_speeds()
  {
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

    Message msg(SET_MOTORS);
    msg.set_motors = payload;
    return msg;
  }

  Message read_turn()
  {
    bool timeout;
    uint8 is_clockwise = reader.next(timeout);
    if (timeout) return NO_MESSAGE;

    uint8 speed = reader.next(timeout);
    if (timeout) return NO_MESSAGE;

    TurnPayload payload;
    payload.is_clockwise = is_clockwise;
    payload.speed = speed;

    Message msg(TURN);
    msg.turn = payload;
    return msg;
  }

  Message read_move_lr()
  {
    bool timeout;
    uint8 is_left = reader.next(timeout);
    if (timeout) return NO_MESSAGE;

    uint8 speed = reader.next(timeout);
    if (timeout) return NO_MESSAGE;

    MoveLRPayload payload;
    payload.is_left = is_left;
    payload.speed = speed;

    Message msg(MOVE_LR);
    msg.move_lr = payload;
    return msg;
  }
};