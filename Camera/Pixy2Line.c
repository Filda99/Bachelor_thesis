//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This file is for defining the Block struct and the Pixy template class version 2.
// (TPixy2).  TPixy takes a communication link as a template parameter so that
// all communication modes (SPI, I2C and UART) can share the same code.
//

#include "Pixy2Line.h"
#include "Pixy2Packet.h"
#include <time.h>

void delay(int miliseconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + miliseconds)
        ;
}

int8_t getFeatures(uint8_t type,  uint8_t features, bool wait)
{
  int8_t res;
  uint8_t offset, fsize, ftype, *fdata;
  struct Packet packet;
  uint8_t numVectors = 0;
  uint8_t numIntersections = 0;
  uint8_t numBarcodes = 0;

  while(1)
  {
    // fill in request data
	packet.m_length = 2;
    packet.m_type = LINE_REQUEST_GET_FEATURES;
    packet.m_bufPayload[0] = type;
    packet.m_bufPayload[1] = features;

    // send request
    sendPacket(packet);
    if (recvPacket(packet)==0)
    {
      if (packet.m_type==LINE_RESPONSE_GET_FEATURES)
      {
        // parse line response
		    for (offset=0, res=0; packet.m_length>offset; offset+=fsize+2)
        {
          ftype = packet.m_buf[offset];
          fsize = packet.m_buf[offset+1];
          fdata = &packet.m_buf[offset+2];
          //TODO fix types
          if (ftype==LINE_VECTOR)
          {
            //vectors = (Vector *)fdata;
            numVectors = fsize/sizeof(struct Vector);
            res |= LINE_VECTOR;
		  }
		  else if (ftype==LINE_INTERSECTION)
          {
            //intersections = (Intersection *)fdata;
            numIntersections = fsize/sizeof(struct Intersection);
            res |= LINE_INTERSECTION;
          }
	      else if (ftype==LINE_BARCODE)
          {
            //barcodes = (Barcode *)fdata;
            numBarcodes = fsize/sizeof(struct Barcode);;
            res |= LINE_BARCODE;
          }
          else
            break; // parse error
        }
        return res;
      }
      else if (packet.m_type==PIXY_TYPE_RESPONSE_ERROR)
      {
		    // if it's not a busy response, return the error
        if ((int8_t)packet.m_buf[0]!=PIXY_RESULT_BUSY)
		      return packet.m_buf[0];
	      else if (!wait) // we're busy
          return PIXY_RESULT_BUSY; // new data not available yet
      }
    }
    else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error

    // If we're waiting for frame data, don't thrash Pixy with requests.
    // We can give up half a millisecond of latency (worst case)
    delay(1);
  }
}

int8_t getMainFeatures(uint8_t features, bool wait)
{
    return getFeatures(LINE_GET_MAIN_FEATURES, features, wait);
};

int8_t getAllFeatures(uint8_t features, bool wait)
{
    return getFeatures(LINE_GET_ALL_FEATURES, features, wait);
};


int8_t setMode(uint8_t mode)
{
  uint32_t res;
  struct Packet packet;
  *(int8_t *)packet.m_bufPayload = mode;
  packet.m_length = 1;
  packet.m_type = LINE_REQUEST_SET_MODE;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}


int8_t setNextTurn(int16_t angle)
{
  uint32_t res;
  struct Packet packet;
  *(int16_t *)packet.m_bufPayload = angle;
  packet.m_length = 2;
  packet.m_type = LINE_REQUEST_SET_NEXT_TURN_ANGLE;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

int8_t setDefaultTurn(int16_t angle)
{
  uint32_t res;
  struct Packet packet;
  *(int16_t *)packet.m_bufPayload = angle;
  packet.m_length = 2;
  packet.m_type = LINE_REQUEST_SET_DEFAULT_TURN_ANGLE;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

int8_t setVector(uint8_t index)
{
  uint32_t res;
  struct Packet packet;
  *(int8_t *)packet.m_bufPayload = index;
  packet.m_length = 1;
  packet.m_type = LINE_REQUEST_SET_VECTOR;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}


int8_t reverseVector()
{
  uint32_t res;
  struct Packet packet;
  packet.m_length = 0;
  packet.m_type = LINE_REQUEST_REVERSE_VECTOR;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}
