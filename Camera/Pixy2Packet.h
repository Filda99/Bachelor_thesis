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
// Main Pixy template class.  This class takes a link class and uses
// it to communicate with Pixy over I2C, SPI, UART or USB using the
// Pixy packet protocol.

#ifndef _PIXY2PACKET_H
#define _PIXY2PACKET_H

#include <stdbool.h>
#include <stdint.h>

// uncomment to turn on debug prints to console
#define PIXY_DEBUG

#define PIXY_DEFAULT_ARGVAL                  0x80000000
#define PIXY_BUFFERSIZE                      0x104
#define PIXY_CHECKSUM_SYNC                   0xc1af
#define PIXY_NO_CHECKSUM_SYNC                0xc1ae
#define PIXY_SEND_HEADER_SIZE                4
#define PIXY_MAX_PROGNAME                    33

#define PIXY_TYPE_REQUEST_CHANGE_PROG        0x02
#define PIXY_TYPE_REQUEST_RESOLUTION         0x0c
#define PIXY_TYPE_RESPONSE_RESOLUTION        0x0d
#define PIXY_TYPE_REQUEST_VERSION            0x0e
#define PIXY_TYPE_RESPONSE_VERSION           0x0f
#define PIXY_TYPE_RESPONSE_RESULT            0x01
#define PIXY_TYPE_RESPONSE_ERROR             0x03
#define PIXY_TYPE_REQUEST_BRIGHTNESS         0x10
#define PIXY_TYPE_REQUEST_SERVO              0x12
#define PIXY_TYPE_REQUEST_LED                0x14
#define PIXY_TYPE_REQUEST_LAMP               0x16
#define PIXY_TYPE_REQUEST_FPS                0x18

#define PIXY_RESULT_OK                       0
#define PIXY_RESULT_ERROR                    -1
#define PIXY_RESULT_BUSY                     -2
#define PIXY_RESULT_CHECKSUM_ERROR           -3
#define PIXY_RESULT_TIMEOUT                  -4
#define PIXY_RESULT_BUTTON_OVERRIDE          -5
#define PIXY_RESULT_PROG_CHANGING            -6

// RC-servo values
#define PIXY_RCS_MIN_POS                     0
#define PIXY_RCS_MAX_POS                     1000L
#define PIXY_RCS_CENTER_POS                  ((PIXY_RCS_MAX_POS-PIXY_RCS_MIN_POS)/2)



struct Version
{
  uint16_t hardware;
  uint8_t firmwareMajor;
  uint8_t firmwareMinor;
  uint16_t firmwareBuild;
  char firmwareType[10];
};


struct Frame
{
  uint16_t frameWidth;
  uint16_t frameHeight;
};


struct Packet
{

  uint8_t *m_buf;
  uint8_t *m_bufPayload;
  uint8_t m_type;
  uint8_t m_length;
  // if connection exists
  bool m_cs;
};

struct Packet CreatePacket();
int8_t initCam();
int16_t getSync(struct Packet packet);
int16_t recvPacket(struct Packet packet);
int16_t sendPacket(struct Packet packet);
int8_t getVersion();
int8_t getResolution();
int8_t setCameraBrightness(uint8_t brightness);
int8_t setServos(uint16_t s0, uint16_t s1);
int8_t setLED(uint8_t r, uint8_t g, uint8_t b);
int8_t setLamp(uint8_t upper, uint8_t lower);
int8_t getFPS();



#endif
