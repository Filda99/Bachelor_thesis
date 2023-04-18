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
uint8 MMCBuf[MMC_BUFF_SIZE];											  // ����
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
* Description    : MMC�����
* EntryParameter : cmd0 - ����0,cmd5 - ����5,addr - ��д��ַ
* ReturnValue    : None
**************************************************************************************/

uint8 MMCWriteCmd( uint8 cmd, uint32 arg, uint8 crc )
{
 uint16 cnt=512;
 uint8  sta;
 	MMCCS(1);
	Send_Byte( 0xff ); //����д������ʱ
	
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
 
	Send_Byte( 0xff ); //����д������ʱ
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
* Description    : SSP0����400K
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void SSP0LowSpeed(void)
{
    SPI0->BR = 7;
}

/**************************************************************************************
* FunctionName   : SSP0HighSpeed()
* Description    : SSP0����24M
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void SSP0HighSpeed(void)
{
	SPI0->BR = 2;
}

/**************************************************************************************
* FunctionName   : MMCInit()
* Description    : ��ʼ��SD����������״̬
* EntryParameter : None
* ReturnValue    : ���ز���״̬��ʧ��-1���ɹ�-0
**************************************************************************************/
uint8 MMCInit(void)
{
	uint8 i = 0,k = 0,tmp = 0;
	uint16 cnt=0;
	uint8  buff[512];
	
	SSP0LowSpeed();			  // ����
	MMCDelayUs(5000);
	                                                  
  for (i=0; i<0x0F; i++) 	// ����һ��������ʱ������
  {
      Send_Byte(0xFF);                                // ��������74��ʱ��
  }
	// SD����λ��idle��ʼ ѭ������CMD0,֪��SD������0X01������IDLE״̬����ʱ�˳�
	// Send Command CMD0 to SD/SD Card
	do
	{
	    tmp = MMCWriteCmd(CMD0,0x00,0x95);   // CMD0   ��ӦΪ01
		  k++;
	}while ((tmp != 1) && (k < 200));
	 
	if(k == 0)
  {
	  MMCCS(1);	//cs ���ߣ�˵��û����
		Send_Byte(0xFF);
		////printf("\n SD reset fail");
	  return 1;// 
	}		
    //-----------------SD����λ��idle����-----
    //��ȡ��ƬSD�汾��Ϣ
	 tmp = MMCWriteCmd( CMD8,0x1AA,0x87 ); 
	 //printf( "SD_CMD8  return  %d........\n\n", tmp );
	if(tmp == 1)// ˵����2.0��
	{
    cnt=0xffff;
		//������ʼ��ָ��CMD55+ACMD41
   do 
   {
    MMCWriteCmd( CMD55, 0, 0xff );
    tmp = MMCWriteCmd( 41,0x40000000, 0xff);//CMD41   MMCWriteCmd( 41,0x40000000, 0xff);
    cnt--;
   } while ((tmp) && (cnt));
	 //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ
	 tmp = MMCWriteCmd(CMD58, 0, 0 );
   if ( tmp != 0x00 ) 
   {
	  MMCCS(1);	//cs ���ߣ�˵��û����
		//printf( "\nSD_CMD58 return  %d....\n", tmp );
	  return 1;//
   }
//��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
   for ( i = 0; i < 4; i++ ) 
   {
    buff[ i ] = Get_Byte();
   }
	//OCR������ɣ�Ƭѡ�ø�
   MMCCS(1);
	// Send_Byte( 0xff );
   //printf( "OCR return: %x %x %x %x....\n\n", buff[0],buff[1],buff[2],buff[3] );

   if ( buff[0] & 0x40 )
   {
		 SD_Type = SD_TYPE_V2HC;
     //printf( "card is V2.0 SDHC.....\n\n" );
   }
   else {
		 SD_Type = SD_TYPE_V2;
     //printf( "card is V2.0.....\n\n" );

   }	 
   while(MMCWriteCmd(CMD16,512,0xff)!=0);
	 MMCWriteCmd(CMD9,0,0xff);
  }
	SSP0HighSpeed();								   // ����	
	MMCCS(1);	
	return 0;                        
}

/**************************************************************************************
* FunctionName   : MMCCSD_CID()
* Description    : CSD-ָ���źű����룻CID-ͨ�������롣�Ĵ�������Ϊ128
* EntryParameter : cmd - ָ�CMD9 - CSD ; CMD10 - CID��buf - ����
* ReturnValue    : ���ز���״̬��ʧ��-1���ɹ�-0
**************************************************************************************/
uint8 MMCCSD_CID(uint8 cmd, uint8 *buf)
{
	uint8 i;
	
	if (MMCWriteCmd(cmd,0x00,0xFF) != 0x00)
	{
	    return 1;                                    // ��ȡʧ��
	}
	
	for (i=0; i<16; i++)
	{
	    *buf++ = Send_Byte(0xFF);
	}
	
	return 0;                                        // �ɹ���
}

/**************************************************************************************
* FunctionName   : MMCReadSingleBolck()
* Description    : ��ȡһ�������ݵ�buffer������
* EntryParameter : None
* ReturnValue    : ���ز���״̬��ʧ��-1���ɹ�-0
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
	    ;			                                 // �ȴ����ݽ��ܿ�ʼ���ܵ�0xFE��ʾ��ʼ
	}
	
  if(sta == 0)
	{
	  for (i=0; i<512; i++)                            // ��ȡ����
	  {
	    buf[i] = Send_Byte(0xFF);
	  }	
	}

	
	Send_Byte(0xFF);			                         // ȡ��CRC�ֽ�
	Send_Byte(0xFF);
	
	MMCCS(1);
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCReadMultipleBolck()
* Description    : ��ȡһ�������ݵ�buffer������
* EntryParameter : None
* ReturnValue    : ���ز���״̬��ʧ��-1���ɹ�-0
**************************************************************************************/
uint8 MMCReadMultipleBolck(uint32 addr,uint8 *buf,uint8 count)
{
    uint16 i;
   	if(SD_Type!=SD_TYPE_V2HC)
		{
		  addr= addr<<9;
		}
		
	if (MMCWriteCmd(CMD18,addr,0xFF) != 0x00)        // ����CMD18
	{
	    return 1;                                    // ��ȡʧ��
	}
	
	MMCCS(0);
	do
	{
	    while (Send_Byte(0xFF) != 0xFE)
	    {
	        ;			                             // �ȴ����ݽ��ܿ�ʼ���ܵ�0xFE��ʾ��ʼ
	    }
	
	    for (i=0; i<512; i++)                        // ��ȡ����
	    {
	        *buf++ = Send_Byte(0xFF);
	    }
	
	    Send_Byte(0xFF);			                     // ȡ��CRC�ֽ�
	    Send_Byte(0xFF);
	
	}while (--count);
	
	MMCCS(1);
	MMCWriteCmd(CMD12,0x00,0xFF);                    // CMD12����ֹͣ����
	Send_Byte(0xFF);//delay
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCWriteSingleBlock()
* Description    : ��buffer�е�����д��һ��������
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
		
		
	if (MMCWriteCmd(CMD24,addr,0x01) != 0x00)         // ����CMD24��SD����ȥ
	{
	    return 1;                                     // д��ʧ��
	}
	
	MMCCS(0);
	//�ȷ����������ݣ���SD��׼����
	Send_Byte(0xFF);                                    // ��������ֽ�
	Send_Byte(0xFF);
	Send_Byte(0xFF); 
//����ʼ����0XFE	
	Send_Byte(0xFE);  	                // �������ݿ�ʼ��־0XFE
	//��һ��sectore������
	for (i=0; i<512; i++)                             // д������
	{
	    Send_Byte(buf[i]);
	}
	//������Byte��Dummy CRC
	Send_Byte(0xFF);			                          // д��CRC�ֽ�
	Send_Byte(0xFF);
	//�ȴ�SD��Ӧ��
	temp = Send_Byte(0xFF);		                      // ��ȡXXX0 0101�ֽ�
	temp &= 0x1F;
	
	if (temp != 0x05)
	{
		  MMCCS(1);
	    return 1; 				                      // д��ʧ��
	}
	
	//�ȴ��������
	while (Send_Byte(0xFF) == 0x00)
	{
	            retry++;
        
        if(retry>0xfffe)//��ʱ��û��д����ɣ������˳�
        {
            MMCCS(1);
            return 1 ;
            //д�볬ʱ����1
        }
	}
	//д����ɣ�Ƭѡ��1
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
* Description    : ��buffer�е�����д��һ��������
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
	//���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����
	   if(SD_Type!=SD_TYPE_MMC)
    {
        tmp=MMCWriteCmd(CMD23,count,0x00);
    }
//�����д��ָ��
	if (MMCWriteCmd(CMD25,addr,0xFF) != 0x00)        // ����CMD25��SD����ȥ
	{
	    return 1;                                    // д��ʧ��
	}

//׼����ʼ���ݴ���
		MMCCS(0);
	//�ſ����ݣ��ȴ�SD��׼����
	Send_Byte(0xFF);
	Send_Byte(0xFF);
	
	//������N��sectorд���ѭ������
	do
	{
		//����ʼ����0XFC�� �����Ƕ��д��
		Send_Byte(0xFC);                               // Send start block token 0xfc (0x11111100)
		//��һ��sector������
		for (i=0; i<512; i++)                        // д������
		{
		    Send_Byte(*buf++);
		}
		//������Byte��dummy CRC
		Send_Byte(0xFF);			                     // д��CRC�ֽ�
		Send_Byte(0xFF);
		//�ȴ�SD��Ӧ��
		tmp = Send_Byte(0xFF);		                 // ��ȡXXX0 0101�ֽ�
		tmp &= 0x1F;
		if (tmp != 0x05)
		{
			 MMCCS(1);
		    return 1; 				                 // д��ʧ��
		}
		
		//�ȴ�SD��д�����
		while (SD_WaitReady() == 0x01)
		{
			MMCCS(1);
			//�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
	  	 return 1;
		}
	}while (--count);
	
	//���ͽ�����������0XFD
	Send_Byte(0xFD);                                   // send 'stop transmission token'
    if(SD_WaitReady())
    {
        while(1){}
    }
//д����ɣ�Ƭѡ��1
		MMCCS(1);
	Send_Byte(0xFF);
		
	return 0;
}


/**************************************************************************************
*                                         End Of File                  
**************************************************************************************/
