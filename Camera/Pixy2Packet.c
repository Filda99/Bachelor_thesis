#include "Pixy2Packet.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "peripherals/i2c.h"
#include "../common/delay.h"


struct Packet CreatePacket(){
	  // allocate buffer space for send/receive
	struct Packet packet;
	packet.m_buf = (uint8_t *)malloc(PIXY_BUFFERSIZE);
	  // shifted buffer is used for sending, so we have space to write header information
	  packet.m_bufPayload = packet.m_buf + PIXY_SEND_HEADER_SIZE;
	  return packet;

}

int8_t init(uint32_t arg)
{
  uint32_t t0;

  i2cInit(400000);
  // wait for pixy to be ready -- that is, Pixy takes a second or 2 boot up
  // getVersion is an effective "ping".  We timeout after 5s.
  for(t0=millis(); millis()-t0<5000; )
  {
    if (getVersion()>=0) // successful version get -> pixy is ready
	{
      getResolution(); // get resolution so we have it
      return PIXY_RESULT_OK;
    }
    delay_ms(5); // delay for sync
  }
  // timeout
  return PIXY_RESULT_TIMEOUT;
}

int16_t getSync(struct Packet packet)
{
  uint8_t i, j, c, cprev;
  int16_t res;
  uint16_t start;

  // parse bytes until we find sync
  for(i=j=0, cprev=0; true; i++)
  {
    res = i2cRead(0x52,0,&c, 1);
    if (res==PIXY_RESULT_OK)
    {
      // since we're using little endian, previous byte is least significant byte
      start = cprev;
      // current byte is most significant byte
      start |= c << 8;
      cprev = c;
      if (start==PIXY_CHECKSUM_SYNC)
      {
        packet.m_cs = true;
        return PIXY_RESULT_OK;
      }
      if (start==PIXY_NO_CHECKSUM_SYNC)
      {
        packet.m_cs = false;
        return PIXY_RESULT_OK;
      }
    }
	  // If we've read some bytes and no sync, then wait and try again.
	  // And do that several more times before we give up.
	  // Pixy guarantees to respond within 100us.
    if (i>=4)
    {
      if (j>=4)
      {
        return PIXY_RESULT_ERROR;
      }
      delay_ms(1);
      j++;
      i = 0;
    }
  }
}

int16_t recvPacket(struct Packet packet)
{
  uint16_t csCalc, csSerial;
  int32_t res;

  res = getSync(packet);
  if (res<0)
    return res;

  if (packet.m_cs)
  {
	  //TODO fill parameters
    res = i2cRead(0x52,0,packet.m_buf, 4);
    if (res<0)
      return res;

    packet.m_type = packet.m_buf[0];
    packet.m_length = packet.m_buf[1];

    csSerial = *(uint16_t *)&packet.m_buf[2];
    //TODO fix check sum
    res = i2cRead(0x52,0,packet.m_buf, packet.m_length/*, &csCalc*/);
    if (res<0)
      return res;

    if (csSerial!=csCalc)
    {
      return PIXY_RESULT_CHECKSUM_ERROR;
    }
  }
  else
  {
    res = i2cRead(0x52,0,packet.m_buf, 2);
    if (res<0)
      return res;

    packet.m_type = packet.m_buf[0];
    packet.m_length = packet.m_buf[1];

    res = i2cRead(0x52,0,packet.m_buf, packet.m_length);
    if (res<0)
      return res;
  }
  return PIXY_RESULT_OK;
}

int16_t sendPacket(struct Packet packet)
{
  // write header info at beginnig of buffer
  packet.m_buf[0] = PIXY_NO_CHECKSUM_SYNC&0xff;
  packet.m_buf[1] = PIXY_NO_CHECKSUM_SYNC>>8;
  packet.m_buf[2] = packet.m_type;
  packet.m_buf[3] = packet.m_length;
  // send whole thing -- header and data in one call
  for(int i=0;i<packet.m_length+PIXY_SEND_HEADER_SIZE;i++){
	  uint32_t res = i2cWrite(0x52,0,packet.m_buf[i]);
	  if(res!=0){
		  return 1;
	  }
  }
  return 2;
}

int8_t changeProg(const char *prog)
{
  int32_t res;
  struct Packet packet;
  packet = CreatePacket();
  // poll for program to change
  while(1)
  {
    strncpy((char *)packet.m_bufPayload, prog, PIXY_MAX_PROGNAME);
    packet.m_length = PIXY_MAX_PROGNAME;
    packet.m_type = PIXY_TYPE_REQUEST_CHANGE_PROG;
    sendPacket(packet);
    if (recvPacket(packet)==0)
    {
      res = *(uint32_t *)packet.m_buf;
      if (res>0)
	  {
        getResolution();  // get resolution so we have it
        return PIXY_RESULT_OK; // success
      }
    }
    else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
    delay_ms(1);
  }
}

int8_t getVersion()
{
	struct Version version;
	//version = NULL;
	struct Packet packet;
	packet=CreatePacket();
  packet.m_length = 0;
  packet.m_type = PIXY_TYPE_REQUEST_VERSION;
  sendPacket(packet);
  if (recvPacket(packet)==0)
  {
    if (packet.m_type==PIXY_TYPE_RESPONSE_VERSION)
    {

    	//TODO copy correct part
      //version = packet.m_buf;
      return packet.m_length;
    }
    else if (packet.m_type==PIXY_TYPE_RESPONSE_ERROR)
      return PIXY_RESULT_BUSY;
  }
  return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

int8_t getResolution()
{
	struct Packet packet;
	packet=CreatePacket();
	struct Frame frame;
	frame.frameWidth = frame.frameHeight = 0;
  packet.m_length = 1;
  packet.m_bufPayload[0] = 0; // for future types of queries
  packet.m_type = PIXY_TYPE_REQUEST_RESOLUTION;
  sendPacket(packet);
  if (recvPacket(packet)==0)
  {
    if (packet.m_type==PIXY_TYPE_RESPONSE_RESOLUTION)
    {
    	//TODO frame
      frame.frameWidth = *(uint16_t *)packet.m_buf;
      frame.frameHeight = *(uint16_t *)(packet.m_buf+sizeof(uint16_t));
      return PIXY_RESULT_OK; // success
    }
    else
      return PIXY_RESULT_ERROR;
  }
  else
    return PIXY_RESULT_ERROR;  // some kind of bitstream error
}


int8_t setCameraBrightness(uint8_t brightness)
{
  uint32_t res;
  struct Packet packet;
  packet=CreatePacket();
  packet.m_bufPayload[0] = brightness;
  packet.m_length = 1;
  packet.m_type = PIXY_TYPE_REQUEST_BRIGHTNESS;
  sendPacket(packet);
  if (recvPacket(packet)==0) // && m_type==PIXY_TYPE_RESPONSE_RESULT && m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}


int8_t setServos(uint16_t s0, uint16_t s1)
{
  uint32_t res;
  struct Packet packet;
  packet=CreatePacket();
  *(int16_t *)(packet.m_bufPayload + 0) = s0;
  *(int16_t *)(packet.m_bufPayload + 2) = s1;
  packet.m_length = 4;
  packet.m_type = PIXY_TYPE_REQUEST_SERVO;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}


int8_t setLED(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t res;
  struct Packet packet;
  packet=CreatePacket();
  packet.m_bufPayload[0] = r;
  packet.m_bufPayload[1] = g;
  packet.m_bufPayload[2] = b;
  packet.m_length = 3;
  packet.m_type = PIXY_TYPE_REQUEST_LED;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

int8_t setLamp(uint8_t upper, uint8_t lower)
{
  uint32_t res;
  struct Packet packet;
  packet=CreatePacket();
  packet.m_bufPayload[0] = upper;
  packet.m_bufPayload[1] = lower;
  packet.m_length = 2;
  packet.m_type = PIXY_TYPE_REQUEST_LAMP;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

int8_t getFPS()
{
  uint32_t res;
  struct Packet packet;
  packet=CreatePacket();
  packet.m_length = 0; // no args
  packet.m_type = PIXY_TYPE_REQUEST_FPS;
  sendPacket(packet);
  if (recvPacket(packet)==0 && packet.m_type==PIXY_TYPE_RESPONSE_RESULT && packet.m_length==4)
  {
    res = *(uint32_t *)packet.m_buf;
    return (int8_t)res;
  }
  else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

