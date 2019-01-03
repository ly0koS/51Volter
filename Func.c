#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

#define key P2

sbit erflags=PSW^5;

extern data uchar  param[9];
extern data uint para1,para2;


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
void read24c02(void)
{
	uchar temp,i;
	sen:	erflags=0;
		Start();
		SendByte(0x90); 				
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
		SendByte(0x90); 				
		cAck( );							
		if(erflags==1) goto sen;			
		SendByte(0x00); 					
		cAck( );							
		if(erflags==1) goto sen;	
	for(i=0;i<8;i++)
	{
		SendByte(param[i]); 						
		cAck( );
	}
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
	return temp;
}