#include "DISP.h"
#include "I2C.h"
#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

sbit recout=P1^0;

bit erflags;
bit setup;
bit selectwp;
bit rise;
bit setupend;

data uchar  param[9];
data uchar  p;
data uchar  hide;
data uint para1,para2;
data uchar da;
data uchar so;


int main()
{
	int keynum;
	EA=1;
	ET0=1;
	ET1=1;
	PT0=1;
	PT1=0;
	TH0=0xfe;
	TL0=0x0c;
	TR0=1;
	TR1=1;
	while(1)
	{
		read24c02();
		display(0,15);
		keynum=keyscan();
		if(keynum==12)
		{
			if(setup==1)
			{
				if(setupend==1)
				{
					write24c02;
					Calculate();
				}
				setup=0;
				display(0,15);
			}
			else
			{
				setup=1;
				display(5,15);
			}
		}
	}
	return 0;
}


void output(void) interrupt 3
{
	
	TR1=0;
	if(param[6]==1)
	{
		TH1=para1/256;
		TL1=para1%256;
		if(rise)
		{
			if(da<240)
			{
				da=da+16;
			}
			else if(da==240)
			{
				da=255;
				rise=0;
			}
		}
		else
		{
			if(da>16)
			{
				da=da-16;
			}
			else
			{
				da=0;
				rise=1;
			}
		}
		so=para2*da/50;
		/*Add I2C DAC Code*/
		sen:	erflags=0;						  
			Start( ); 							
			SendByte(0x90); 				
			cAck( );							
			if(erflags==1) goto sen;			
			SendByte(0x00); 					
			cAck( );							
			if(erflags==1) goto sen;	
		SendByte(so);
		cAck();
	}
	else if(param[6]==2)
	{
		recout=~recout;
		if(recout)  
		{
			TH1=para1/256;
			TL1=para1%256; 
		}
		else	   
		{
			TH1=para2/256;
			TL1=para2%256; 
		}
	}
	TR1=1;
}