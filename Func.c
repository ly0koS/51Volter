#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

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
uchar read24c02(void)
{
	uchar temp;
	if(temp==0x55)
	{
		
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