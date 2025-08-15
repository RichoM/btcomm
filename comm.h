#pragma once
#include "Arduino.h"
#include "types.h"
#include "reader.h"

enum MessageType 
{
  NO_MESSAGE = 0,

  SET_MOTORS = 1,
  TURN = 2,
  MOVE_LR = 3,
};

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

struct Message 
{
    MessageType type = NO_MESSAGE;
    union 
    {
        SetMotorsPayload set_motors = {0};
        TurnPayload turn;
        MoveLRPayload move_lr;
    };
};

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

  bool check_incoming(Message* msg)
  {
    if (!reader.available()) return false;

    bool timeout;
    uint8 in = reader.next(timeout);

    if (timeout) return false;

    if (in == SET_MOTORS)
    {
      return read_set_motor_speeds(msg);
    }
    else if (in == TURN)
    {
      return read_turn(msg);
    }
    else if (in == MOVE_LR)
    {
      return read_move_lr(msg);
    }
  }

  bool read_set_motor_speeds(Message* msg)
  {
    bool timeout;

    uint8 directions = reader.next(timeout);
    if (timeout) return false;

    msg->set_motors.direction[0] = directions & 1;
    msg->set_motors.direction[1] = directions & 2;
    msg->set_motors.direction[2] = directions & 4;
    msg->set_motors.direction[3] = directions & 8;

    msg->set_motors.speed[0] = reader.next(timeout);
    if (timeout) return false;

    msg->set_motors.speed[1] = reader.next(timeout);
    if (timeout) return false;

    msg->set_motors.speed[2] = reader.next(timeout);
    if (timeout) return false;

    msg->set_motors.speed[3] = reader.next(timeout);
    if (timeout) return false;

    msg->type = SET_MOTORS;
    return true;
  }

  bool read_turn(Message* msg)
  {
    bool timeout;

    msg->turn.is_clockwise = reader.next(timeout);
    if (timeout) return false;

    msg->turn.speed = reader.next(timeout);
    if (timeout) return false;

    msg->type = TURN;
    return true;
  }

  bool read_move_lr(Message* msg)
  {
    bool timeout;

    msg->move_lr.is_left = reader.next(timeout);
    if (timeout) return false;

    msg->move_lr.speed = reader.next(timeout);
    if (timeout) return false;

    msg->type = MOVE_LR;
    return true;
  }
};