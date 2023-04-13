/****************************************Copyright (c)****************************************************
**               Copyright �2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved
**
**                                 http://www.ucdragon.com
**

**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_spi.c
** Last modified date:    
** Last version:            V1.0
** Descriptions:            spi function
**
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "spi.h"
#include "fsl_spi.h"



uint8 Send_Byte (uint8 data)
{
	spi_transfer_t xfer = {0};
	xfer.txData = &data;
	xfer.rxData = NULL;
	xfer.dataSize = 1U; // One byte
	SPI_MasterTransferBlocking(SPI0, &xfer);

	uint8 ucTemp;
	ucTemp = SPI0->D;                                                    /* 清空接收标记寄存器           */
	ucTemp = ucTemp;
	return ucTemp;
}

/*********************************************************************************************************
** 函数名称:Get_Byte
** 函数功能:SPI接口接收一个字节数据 使用SPI0
** 输入参数:无
** 输出参数:无
*********************************************************************************************************/
uint8  Get_Byte (void)
{
    uint8 ucTemp;
    while((SPI0->S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);             /* 发送前要先判断寄存器         */
    SPI0->D = 0xff;                                                      /* 发送空数据读取内容           */

    while((SPI0->S & SPI_S_SPRF_MASK) != SPI_S_SPRF_MASK);               /* 接收数据有效                 */
    ucTemp = SPI0->D;
    return (ucTemp);                                             /* 返回接收到的数据             */
}

/*********************************************************************************************************
** Function name:           spiTX
** Descriptions:            spi初始化
** input parameters:        SPI输入基地址 查询方式发送数据
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:           
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void spiTX(uint8 ucData)
{
    while((SPI0->S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);             /* 发送前要先判断寄存器         */
    SPI0->D = ucData;
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/



