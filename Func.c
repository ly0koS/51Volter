#include "Func.h"
#define uchar unsigned char

sbit erflags=PSW^5;

extern data uchar  param[9];

void Calculate(uchar in)
{
	
}
uchar read24c02(void)
{
	uchar temp;
	if(temp==0x55)
	{
		
	}
	else
	{
		
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