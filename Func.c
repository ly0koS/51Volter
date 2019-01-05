#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

#define key P2

sbit erflags=PSW^5;

extern data uchar  param[9];
extern data uint para1,para2;
extern uchar disp[8];
extern uchar d2;
extern uchar k;

void Calculate()
{
	uint g,g1,g2;
	uint freq;
	if(param[6]==1)
	{
		g=10^6/param[1]/32;
		para1=65536-g;
		para2=param[2];
	}
	if(param[6]==2)
	{
		freq=param[3]*1000+param[4];
		g1=10^4/freq*param[5];
		g2=10^4/freq*(100-param[5]);
		para1=65536-g1;
		para2=65546-g2;
	}
}

void ParamProcess()
{
	uchar freq;
	if(param[6]==1)
	{
		param[1]=disp[6]*10+disp[7];
	}
	else if(param[6]==2&&d2==15)
	{
		freq=disp[4]*1000+disp[5]*100+disp[6]*10+disp[7];
		param[7]=freq/256,param[8]=freq%256;
	}
	else
		param[disp[1]*2]=disp[6]*10+disp[7];
}

void NumProcess(unsigned int keynum)
{
	uchar i;
	if(k>=4)
	{
		if(param[6]==2&d2==15)																//Square Wave-Frequence Mode
		{
			if(k==4)
			{
				if(keynum==1)																			//Max=1000Hz
				{
					disp[k]=keynum;
					for(i=5;i<8;i++)
					{
						disp[i]=0;
					}
				}
				else if(keynum==0)
					disp[k]=keynum;
			}
			else
			{
				if(disp[4]==0)
				{
					disp[4]=16;																				//disp[4] off
					disp[k]=keynum;
				}
			}
		}
		else if(k==6)
		{
			if(param[6]==2&d2==10)
			{
				if(keynum>=2&&keynum<=7)
					disp[k]=keynum;
			}
			else
			{
				if(keynum>=1&&keynum<5)															//Sin Wave-Frequence Mode
					disp[k]=keynum;
				else if(keynum==5)																	//Max=50Hz
				{
					disp[k]=keynum;
					disp[7]=0;
				}
			}
		}
		else																										//maybe else if?
		{
			if(param[6]==1&d2==10&&disp[6]==5)										//Max=50Hz
			{
				disp[7]=0;
			}
			else
				disp[k]=keynum;
		}
	}
}

void read24c02(void)
{
	uchar temp,i;
	sen:	erflags=0;
		Start();
		SendByte(0xa0); 				
		cAck( );							
		if(erflags==1) goto sen;			
		SendByte(0x00); 					
		cAck( );							
		if(erflags==1) goto sen;
	temp=RcvByte();
	Ack();
	if(temp==0x55)
	{
		for(i=1;i<8;i++)
		{
			param[i]=RcvByte();
			cAck();
		}
	}
	else
	{
		write24c02();
	}
}
void write24c02(void)
{
	uchar i;
	sen:	erflags=0;						  
		Start( ); 							
		SendByte(0xa0); 				
		cAck( );							
		if(erflags==1) goto sen;			
		SendByte(0x00); 					
		cAck( );							
		if(erflags==1) goto sen;
		SendByte(0xa1);
		cAck();
	for(i=0;i<8;i++)
	{
		SendByte(param[i]); 						
		cAck( );
	}
	Stop();
}

int keyscan()
{
	uchar t,temp;
	key=0xf0;           
  if(key!=0xf0)
  {
		delay(10);      
		if(key!=0xf0)
		{
			 switch(key)
			 {
				 case(0xe0):temp=0;break;
				 case(0xd0):temp=1;break;
				 case(0xb0):temp=2;break;
				 case(0x70):temp=3;break;
			}
			key=0x0f;
			delay(10);
			switch(key)
			{
				case(0x0e):temp=temp;break;
				case(0x0d):temp=temp+4;break;
				case(0x0b):temp=temp+8;break;
				case(0x07):temp=temp+12;break;
			}
		}
  }
	switch(temp)														//To fit keymap
	{
		case(8):temp=110;break;								//go left
		case(9):temp=8;break;									//fit keymap
		case(10):temp=9;break;								//fit keymap
		case(11):temp=120;break;							//go right
	}
	return temp;
}