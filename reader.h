#pragma once
#include "types.h"
#include "Arduino.h"

#define TIMEOUT		100

class Reader
{
public:
	uint16 counter;

	virtual uint8 next(bool&) = 0;

	virtual int32 nextLong(int16 size, bool& timeout)
  {
    int32 result = 0;	
    for (int32 i = size - 1; i >= 0; i--)
    {
      result |= ((uint32)next(timeout) << (i * 8));
      if (timeout) return 0;
    }
    return result;
  }

  /*
  INFO(Richo): Code taken and adapted from
  http://www.microchip.com/forums/m590535.aspx#590570
  */
	virtual float nextFloat(bool& timeout)
  {
    uint32 a = next(timeout);
    if (timeout) return 0;
    uint32 b = next(timeout);
    if (timeout) return 0;
    uint32 c = next(timeout);
    if (timeout) return 0;
    uint32 d = next(timeout);
    if (timeout) return 0;

    uint32 value = (a << 24) | (b << 16) | (c << 8) | d;
    return uint32_to_float(value);
  }
};

class StreamReader : public Reader
{
private:
	Stream* stream;

public:
	StreamReader(Stream* s) { stream = s; }

  uint8 available() { return stream->available(); }
	
  uint8 next(bool& timeout)
  {
    int32 start = millis();
    timeout = false;
    while (stream->available() <= 0)
    {
      timeout = millis() - start > TIMEOUT;
      if (timeout) return 0;
    }
    counter++;
    return (uint8)stream->read();
  }
};