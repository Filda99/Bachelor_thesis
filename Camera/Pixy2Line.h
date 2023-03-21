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

#ifndef _PIXY2LINE_H
#define _PIXY2LINE_H

#include <stdint.h>
#include <stdbool.h>
#include "Pixy2Packet.h"

#define LINE_REQUEST_GET_FEATURES                0x30
#define LINE_RESPONSE_GET_FEATURES               0x31
#define LINE_REQUEST_SET_MODE                    0x36
#define LINE_REQUEST_SET_VECTOR                  0x38
#define LINE_REQUEST_SET_NEXT_TURN_ANGLE         0x3a
#define LINE_REQUEST_SET_DEFAULT_TURN_ANGLE      0x3c
#define LINE_REQUEST_REVERSE_VECTOR              0x3e

#define LINE_GET_MAIN_FEATURES                   0x00
#define LINE_GET_ALL_FEATURES                    0x01

#define LINE_MODE_TURN_DELAYED                   0x01
#define LINE_MODE_MANUAL_SELECT_VECTOR           0x02
#define LINE_MODE_WHITE_LINE                     0x80

// features
#define LINE_VECTOR                              0x01
#define LINE_INTERSECTION                        0x02
#define LINE_BARCODE                             0x04
#define LINE_ALL_FEATURES                        (LINE_VECTOR | LINE_INTERSECTION | LINE_BARCODE)

#define LINE_FLAG_INVALID                        0x02
#define LINE_FLAG_INTERSECTION_PRESENT           0x04

#define LINE_MAX_INTERSECTION_LINES              6

struct Vector
{

  uint8_t m_x0;
  uint8_t m_y0;
  uint8_t m_x1;
  uint8_t m_y1;
  uint8_t m_index;
  uint8_t m_flags;
  uint8_t numVectors;
};

struct IntersectionLine
{
  uint8_t m_index;
  uint8_t m_reserved;
  int16_t m_angle;

};

struct Intersection
{

  uint8_t m_x;
  uint8_t m_y;
  uint8_t numIntersections;
  uint8_t m_n;
  uint8_t m_reserved;
  struct IntersectionLine m_intLines[LINE_MAX_INTERSECTION_LINES];
};

struct Barcode
{
  uint8_t m_x;
  uint8_t m_y;
  uint8_t m_flags;
  uint8_t m_code;
  uint8_t numBarcodes;
};

void delay(int miliseconds);
int8_t getFeatures(uint8_t type,  uint8_t features, bool wait);
int8_t getMainFeatures(uint8_t features, bool wait);
int8_t getAllFeatures(uint8_t features, bool wait);
int8_t setMode(uint8_t mode);
int8_t setNextTurn(int16_t angle);
int8_t setDefaultTurn(int16_t angle);
int8_t setVector(uint8_t index);
int8_t reverseVector();







#endif
