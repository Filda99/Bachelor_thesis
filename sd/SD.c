/************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : sd.c
* Version        : V1.0
* Programmer(s)  : 
* Parameters     :KL26
* DebugTools     : JLINK(SWD) And Realview MDK
* Description    : SD driver
*
*
**************************************************************************************/



/**************************************************************************************
* header file                                                 
**************************************************************************************/
#include "SD.h"

/**************************************************************************************
* global variable                                              
**************************************************************************************/

uint8 SD_Type =0;
uint32 Logic_sector;
uint8 MMCBuf[MMC_BUFF_SIZE];											  // 缓存
/**************************************************************************************
* FunctionName   : MMCDelayUs()
* Description    :software delay
* EntryParameter : tt
* ReturnValue    : None
**************************************************************************************/
void MMCDelayUs(uint16 tt)
{
	uint8 i;

    while (tt--)
	{
        for (i=0; i<250; i++)
        {
        	;
        }
	}
}

/**************************************************************************************
* FunctionName   : MMCCS()
* Description    : MMC card selection: 0 select ; 1 not select
* EntryParameter : cs 
* ReturnValue    : None
**************************************************************************************/
void MMCCS(uint8 cs)
{
	if (cs == 1)
	{
	   MMC_CS_SET();                                   // SS=1 
	}
	else
	{
	   MMC_CS_CLR();                                   // SS=0 
	}
}

uint8  sd_WaitRead( void )
{
 uint32 cnt = 0x00fffff;
 uint8  sta;
 do 
 {
  sta = Send_Byte( 0xff );;
  if ( sta == 0xff ) // 
  {
   return 0;
  }
  cnt--;
 } while ( cnt );

 return 1;

}
__inline static void sd_Disable_Select( void )
{
 MMCCS(1);    // 
 Send_Byte( 0xff ); // 
}

__inline static uint8 sd_Enable_Select( void )
{
 MMCCS(0);     // 
 if ( sd_WaitRead() == 0 ) // ??SD/MMC??
 {
  return 0;
 }
 sd_Disable_Select();  

 return 1;     
}


/**************************************************************************************
* FunctionName   : MMCWriteCmd()
* Description    : MMC命令发送
* EntryParameter : cmd0 - 命令0,cmd5 - 命令5,addr - 读写地址
* ReturnValue    : None
**************************************************************************************/

uint8 MMCWriteCmd( uint8 cmd, uint32 arg, uint8 crc )
{
 uint16 cnt=512;
 uint8  sta;
 	MMCCS(1);
	Send_Byte( 0xff ); //高速写命令延时
	
	MMCCS(0);
 if ( sd_Enable_Select() ) // ?????
 {
  return 0xff;   // ??????,????255
 }
 
	
	Send_Byte( cmd | 0x40 );  // ??????
	Send_Byte( (uint8)(arg>>24) ); // ??1
	Send_Byte( (uint8)(arg>>16) ); // ??2
	Send_Byte( (uint8)(arg>>8) ); // ??3
	Send_Byte( (uint8)(arg) );  // ??4
	Send_Byte( crc );   // CRC???

 do 
 {
  sta = Send_Byte( 0xff ); // ??????
  cnt--;
 } while ( (cnt)&&(sta==0xff) );

 return sta; // ??????    
}

uint8 MMCWriteCmd_NoDessert( uint8 cmd, uint32 arg, uint8 crc )
{
 uint16 cnt=512;
 uint8  sta;
 
	Send_Byte( 0xff ); //高速写命令延时
	Send_Byte( 0xff ); 
	
	MMCCS(0);
	Send_Byte( cmd | 0x40 );  // ??????
	Send_Byte( (uint8)(arg>>24) ); // ??1
	Send_Byte( (uint8)(arg>>16) ); // ??2
	Send_Byte( (uint8)(arg>>8) ); // ??3
	Send_Byte( (uint8)(arg) );  // ??4
	Send_Byte( crc );   // CRC???

 do 
 {
  sta = Send_Byte( 0xff ); // ??????
  cnt--;
 } while ( (cnt)&&(sta==0xff) );

 return sta; // ??????    
}
/**************************************************************************************
* FunctionName   : SSP0LowSpeed()
* Description    : SSP0低速400K
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void SSP0LowSpeed(void)
{
    SPI0->BR = 7;
}

/**************************************************************************************
* FunctionName   : SSP0HighSpeed()
* Description    : SSP0高速24M
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void SSP0HighSpeed(void)
{
	SPI0->BR = 2;
}

/**************************************************************************************
* FunctionName   : MMCInit()
* Description    : 初始化SD卡，并返回状态
* EntryParameter : None
* ReturnValue    : 返回操作状态：失败-1；成功-0
**************************************************************************************/
uint8 MMCInit(void)
{
	uint8 i = 0,k = 0,tmp = 0;
	uint16 cnt=0;
	uint8  buff[512];
	
	SSP0LowSpeed();			  // 低速
	MMCDelayUs(5000);
	                                                  
  for (i=0; i<0x0F; i++) 	// 发送一定数量的时钟脉冲
  {
      Send_Byte(0xFF);                                // 发送至少74个时钟
  }
	// SD卡复位到idle开始 循环发送CMD0,知道SD卡返回0X01，进入IDLE状态，超时退出
	// Send Command CMD0 to SD/SD Card
	do
	{
	    tmp = MMCWriteCmd(CMD0,0x00,0x95);   // CMD0   响应为01
		  k++;
	}while ((tmp != 1) && (k < 200));
	 
	if(k == 0)
  {
	  MMCCS(1);	//cs 拉高，说明没连上
		Send_Byte(0xFF);
		printf("\n SD reset fail");
	  return 1;// 
	}		
    //-----------------SD卡复位到idle结束-----
    //获取卡片SD版本信息
	 tmp = MMCWriteCmd( CMD8,0x1AA,0x87 ); 
	 printf( "SD_CMD8  return  %d........\n\n", tmp );  
	if(tmp == 1)// 说明是2.0卡
	{
    cnt=0xffff;
		//发卡初始化指令CMD55+ACMD41
   do 
   {
    MMCWriteCmd( CMD55, 0, 0xff );
    tmp = MMCWriteCmd( 41,0x40000000, 0xff);//CMD41   MMCWriteCmd( 41,0x40000000, 0xff);
    cnt--;
   } while ((tmp) && (cnt));
	 //初始化指令发送完成，接下来获取OCR信息
	 tmp = MMCWriteCmd(CMD58, 0, 0 );
   if ( tmp != 0x00 ) 
   {
	  MMCCS(1);	//cs 拉高，说明没连上
		printf( "\nSD_CMD58 return  %d....\n", tmp );
	  return 1;//
   }
//读OCR指令发出后，紧接着是4字节的OCR信息
   for ( i = 0; i < 4; i++ ) 
   {
    buff[ i ] = Get_Byte();
   }
	//OCR接收完成，片选置高
   MMCCS(1);
	// Send_Byte( 0xff );
   printf( "OCR return: %x %x %x %x....\n\n", buff[0],buff[1],buff[2],buff[3] );

   if ( buff[0] & 0x40 )
   {
		 SD_Type = SD_TYPE_V2HC;
     printf( "card is V2.0 SDHC.....\n\n" );
   }
   else {
		 SD_Type = SD_TYPE_V2;
     printf( "card is V2.0.....\n\n" );

   }	 
   while(MMCWriteCmd(CMD16,512,0xff)!=0);
	 MMCWriteCmd(CMD9,0,0xff);
  }
	SSP0HighSpeed();								   // 高速	
	MMCCS(1);	
	return 0;                        
}

/**************************************************************************************
* FunctionName   : MMCCSD_CID()
* Description    : CSD-指令信号编译码；CID-通道编译码。寄存器长度为128
* EntryParameter : cmd - 指令：CMD9 - CSD ; CMD10 - CID。buf - 缓冲
* ReturnValue    : 返回操作状态：失败-1；成功-0
**************************************************************************************/
uint8 MMCCSD_CID(uint8 cmd, uint8 *buf)
{
	uint8 i;
	
	if (MMCWriteCmd(cmd,0x00,0xFF) != 0x00)
	{
	    return 1;                                    // 读取失败
	}
	
	for (i=0; i<16; i++)
	{
	    *buf++ = Send_Byte(0xFF);
	}
	
	return 0;                                        // 成功返
}

/**************************************************************************************
* FunctionName   : MMCReadSingleBolck()
* Description    : 读取一扇区数据到buffer缓冲中
* EntryParameter : None
* ReturnValue    : 返回操作状态：失败-1；成功-0
**************************************************************************************/
uint8 MMCReadSingleBolck(uint32 addr,uint8 *buf)
{
	uint16 i;
	uint8 sta;
	if(SD_Type!=SD_TYPE_V2HC)
		{
		  addr= addr<<9;
		}
	
	sta = MMCWriteCmd(CMD17,addr,0x01);
	while(sta !=0)
	{
	  sta = MMCWriteCmd(CMD17,addr,0x01);
	}

	while (Get_Byte() != 0xFE)
	{
	    ;			                                 // 等待数据接受开始，受到0xFE表示开始
	}
	
  if(sta == 0)
	{
	  for (i=0; i<512; i++)                            // 读取数据
	  {
	    buf[i] = Send_Byte(0xFF);
	  }	
	}

	
	Send_Byte(0xFF);			                         // 取走CRC字节
	Send_Byte(0xFF);
	
	MMCCS(1);
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCReadMultipleBolck()
* Description    : 读取一扇区数据到buffer缓冲中
* EntryParameter : None
* ReturnValue    : 返回操作状态：失败-1；成功-0
**************************************************************************************/
uint8 MMCReadMultipleBolck(uint32 addr,uint8 *buf,uint8 count)
{
    uint16 i;
   	if(SD_Type!=SD_TYPE_V2HC)
		{
		  addr= addr<<9;
		}
		
	if (MMCWriteCmd(CMD18,addr,0xFF) != 0x00)        // 发送CMD18
	{
	    return 1;                                    // 读取失败
	}
	
	MMCCS(0);
	do
	{
	    while (Send_Byte(0xFF) != 0xFE)
	    {
	        ;			                             // 等待数据接受开始，受到0xFE表示开始
	    }
	
	    for (i=0; i<512; i++)                        // 读取数据
	    {
	        *buf++ = Send_Byte(0xFF);
	    }
	
	    Send_Byte(0xFF);			                     // 取走CRC字节
	    Send_Byte(0xFF);
	
	}while (--count);
	
	MMCCS(1);
	MMCWriteCmd(CMD12,0x00,0xFF);                    // CMD12发送停止命令
	Send_Byte(0xFF);//delay
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCWriteSingleBlock()
* Description    : 把buffer中的数据写入一个扇区中
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint8 MMCWriteSingleBlock(uint32 addr,const uint8 *buf)
{
	uint16 i,retry ;
	uint8  temp;
	
	if(SD_Type!=SD_TYPE_V2HC)
    {
        addr=addr<<9 ;
    }
		
		
	if (MMCWriteCmd(CMD24,addr,0x01) != 0x00)         // 发送CMD24到SD卡中去
	{
	    return 1;                                     // 写入失败
	}
	
	MMCCS(0);
	//先放三个空数据，等SD卡准备好
	Send_Byte(0xFF);                                    // 发送填冲字节
	Send_Byte(0xFF);
	Send_Byte(0xFF); 
//放起始令牌0XFE	
	Send_Byte(0xFE);  	                // 发送数据开始标志0XFE
	//放一个sectore的数据
	for (i=0; i<512; i++)                             // 写入数据
	{
	    Send_Byte(buf[i]);
	}
	//发两个Byte的Dummy CRC
	Send_Byte(0xFF);			                          // 写入CRC字节
	Send_Byte(0xFF);
	//等待SD卡应答
	temp = Send_Byte(0xFF);		                      // 读取XXX0 0101字节
	temp &= 0x1F;
	
	if (temp != 0x05)
	{
		  MMCCS(1);
	    return 1; 				                      // 写入失败
	}
	
	//等待操作完成
	while (Send_Byte(0xFF) == 0x00)
	{
	            retry++;
        
        if(retry>0xfffe)//长时间没有写入完成，报错退出
        {
            MMCCS(1);
            return 1 ;
            //写入超时返回1
        }
	}
	//写入完成，片选置1
	MMCCS(1);
	Send_Byte(0xFF);
	return 0;
}

uint8 SD_WaitReady(void)
{
    uint8 r1;
    uint16 retry;
    retry = 0;
    do
    {
        r1 = Send_Byte(0xFF);
        if(retry==0xfffe)return 1; 
    }while(r1!=0xFF); 
    return 0;
}


/**************************************************************************************
* FunctionName   : MMCWriteMultipleBlock()
* Description    : 把buffer中的数据写入一个扇区中
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint8 MMCWriteMultipleBlock(uint32 addr,const uint8 *buf,uint8 count)
{
	uint16 i;
	uint8  tmp;
	if(SD_Type!=SD_TYPE_V2HC)
		{
		  addr= addr<<9;
		}
	//如果目标卡不是MMC卡，启用ACMD23指令使能预擦除
	   if(SD_Type!=SD_TYPE_MMC)
    {
        tmp=MMCWriteCmd(CMD23,count,0x00);
    }
//发多块写入指令
	if (MMCWriteCmd(CMD25,addr,0xFF) != 0x00)        // 发送CMD25到SD卡中去
	{
	    return 1;                                    // 写入失败
	}

//准备开始数据传输
		MMCCS(0);
	//放空数据，等待SD卡准备好
	Send_Byte(0xFF);
	Send_Byte(0xFF);
	
	//下面是N个sector写入的循环部分
	do
	{
		//放起始令牌0XFC， 表面是多块写入
		Send_Byte(0xFC);                               // Send start block token 0xfc (0x11111100)
		//放一个sector的数据
		for (i=0; i<512; i++)                        // 写入数据
		{
		    Send_Byte(*buf++);
		}
		//发两个Byte的dummy CRC
		Send_Byte(0xFF);			                     // 写入CRC字节
		Send_Byte(0xFF);
		//等待SD卡应答
		tmp = Send_Byte(0xFF);		                 // 读取XXX0 0101字节
		tmp &= 0x1F;
		if (tmp != 0x05)
		{
			 MMCCS(1);
		    return 1; 				                 // 写入失败
		}
		
		//等待SD卡写入完成
		while (SD_WaitReady() == 0x01)
		{
			MMCCS(1);
			//等待SD卡写入完成超时，直接退出报错
	  	 return 1;
		}
	}while (--count);
	
	//发送结束传输令牌0XFD
	Send_Byte(0xFD);                                   // send 'stop transmission token'
    if(SD_WaitReady())
    {
        while(1){}
    }
//写入完成，片选置1
		MMCCS(1);
	Send_Byte(0xFF);
		
	return 0;
}


/**************************************************************************************
*                                         End Of File                  
**************************************************************************************/
