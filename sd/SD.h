/************************************************************************************** 
*                       Crazy ARM LPC2132 V1.0 ���ݲɼ�ϵͳ����       
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : sd.h
* Version        : V1.0
* Programmer(s)  : 
* Parameters     : Cortex-M0 LPC1114 12.000000MHz
* DebugTools     : JLINK V8.0 (SWD) And Realview MDK V4.12
* Description    : SD��������
*
*
**************************************************************************************/

#ifndef __SD_H 
#define __SD_H
//#include "common.h"
#include <sd/spi.h>
#include "MKL25Z4.h"
/**************************************************************************************
* ���궨�塿 macro definition                                                 
**************************************************************************************/
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

/**************************************************************************************
*  SD Command
**************************************************************************************/
//#define CMD0	        (0x40+0)	                       /* GO_IDLE_STATE          */
//#define CMD1	        (0x40+1)	                       /* SEND_OP_COND           */
//#define CMD8	        (0x40+8)	                       /* SEND_IF_COND           */
//#define CMD9	        (0x40+9)	                       /* SEND_CSD               */
//#define CMD10	        (0x40+10)	                       /* SEND_CID               */
//#define CMD12	        (0x40+12)	                       /* STOP_TRANSMISSION      */
//#define CMD13	        (0xC0+13)	                       /* SD_STATUS              */
//#define CMD16	        (0x40+16)	                       /* SET_BLOCKLEN           */
//#define CMD17	        (0x40+17)	                       /* READ_SINGLE_BLOCK      */
//#define CMD18	        (0x40+18)	                       /* READ_MULTIPLE_BLOCK    */
//#define CMD23	        (0xC0+23)	                       /* SET_WR_BLK_ERASE_COUNT */
//#define CMD24	        (0x40+24)	                       /* WRITE_BLOCK            */
//#define CMD25	        (0x40+25)	                       /* WRITE_MULTIPLE_BLOCK   */
//#define CMD32	        (0x40+32)	                       /* ERASE_BLOCK_START_ADDR */
//#define CMD33	        (0x40+33)	                       /* ERASE_BLOCK_END_ADDR   */
//#define CMD38	        (0x40+38)	                       /* ERASE_SELECTED_BLOCKS  */
//#define CMD41	        (0xC0+41)	                       /* SEND_OP_COND           */
//#define CMD55	        (0x40+55)	                       /* APP_CMD                */
//#define CMD58	        (0x40+58)	                       /* READ_OCR               */

#define CMD0	        (0)	                       /* GO_IDLE_STATE          */
#define CMD1	        (1)	                       /* SEND_OP_COND           */
#define CMD8	        (8)	                       /* SEND_IF_COND           */
#define CMD9	        (9)	                       /* SEND_CSD               */
#define CMD10	        (10)	                       /* SEND_CID               */
#define CMD12	        (12)	                       /* STOP_TRANSMISSION      */
#define CMD13	        (13)	                       /* SD_STATUS              */
#define CMD16	        (16)	                       /* SET_BLOCKLEN           */
#define CMD17	        (17)	                       /* READ_SINGLE_BLOCK      */
#define CMD18	        (18)	                       /* READ_MULTIPLE_BLOCK    */
#define CMD23	        (23)	                       /* SET_WR_BLK_ERASE_COUNT */
#define CMD24	        (24)	                       /* WRITE_BLOCK            */
#define CMD25	        (25)	                       /* WRITE_MULTIPLE_BLOCK   */
#define CMD32	        (32)	                       /* ERASE_BLOCK_START_ADDR */
#define CMD33	        (33)	                       /* ERASE_BLOCK_END_ADDR   */
#define CMD38	        (38)	                       /* ERASE_SELECTED_BLOCKS  */
#define CMD41	        (41)	                       /* SEND_OP_COND           */
#define CMD55	        (55)	                       /* APP_CMD                */
#define CMD58	        (58)	                       /* READ_OCR               */


#define MMC_CS_SET()      (FGPIOC->PSOR |= 1<<4)
#define MMC_CS_CLR()      (FGPIOC->PCOR |= 1<<4)

/**************************************************************************************
* ��ȫ�ֺ����� global function                                                 
**************************************************************************************/                                                      
uint8 MMCInit(void);                                                   // MMC����ʼ��
//uint8 MMCWriteCmd(uint8 cmd0,uint8 cmd5,uint32 addr);                  // д����
uint8 MMCWriteCmd( uint8 cmd, uint32 arg, uint8 crc );
uint8 MMCCSD_CID(uint8 cmd, uint8 *buf);                               // ��ȡMMC-CID-CSD�Ĵ���
void  MMCCS(uint8 cs);                                                 // MMC��Ƭѡ-1ѡ��/0��ѡ��

uint8 MMCWriteSingleBlock(uint32 addr,const uint8 *buf);               // дһ��������
uint8 MMCReadSingleBolck(uint32 addr,uint8 *buf);                      // ��һ��������
uint8 MMCWriteMultipleBlock(uint32 addr,const uint8 *buf,uint8 count); // д������
uint8 MMCReadMultipleBolck(uint32 addr,uint8 *buf,uint8 count);        // ��������

/**************************************************************************************
* ��ȫ�ֱ����� global variable                                              
**************************************************************************************/
#define MMC_BUFF_SIZE   (512)       // SD buffer size

/*************************************************************************************/
#endif

/**************************************************************************************
*                              End Of File         
**************************************************************************************/
