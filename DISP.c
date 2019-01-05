#include "DISP.h"

sbit OE=P1^3;
sbit LE1=P1^4;
sbit LE2=P1^5;
sbit K1=P2^0;
sbit K2=P2^1;


#define uchar unsigned char
	
uchar disp[8]={16,16,16,16,16,16,16,16};
uchar code sled_bit[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar table[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
uchar i=0;
uchar key=99;
uchar temp;
unsigned int timing=0;

extern data uchar  param[9];
extern data uchar k;
extern data uchar hide;

void display(uchar d1,d2)
{
	disp[0]=d1;
	disp[1]=param[6];
	if(d2==0x0f)
	{
		disp[2]=15;
		switch(param[6])
		{
			case(1):
			{
				disp[3]=16;
				disp[4]=16; 
				disp[5]=16;  
				disp[7]=param[1]%10;
				disp[6]=param[1]/10;
				break;
			}
			case(2):
			{
				disp[3]=16;
				disp[4]=param[3];
				disp[5]=param[4]/100;
				disp[6]=(param[4]/10)%10;
				disp[7]=param[4]%10;
				break;
			}
		}
	}
	else if(d2==0x0a)
	{
		disp[2]=10;
		switch(param[6])
		{
			case(1):
			{
				disp[3]=16;
				disp[4]=16; 
				disp[5]=16;  
				disp[7]=param[2]%10;
				disp[6]=param[2]/10;
				break;
			}
			case(2):
			{
				disp[3]=16;
				disp[4]=16; 
				disp[5]=16; 
				disp[7]=param[5]%10;
				disp[6]=param[5]/10;
				break;
			}
		}
	}
}


void delay(uchar ti)
{
	uchar t;
	while(ti--)
	{
		for(t=0;t<255;t++);
	}
}

void scan() interrupt 1
{
	uchar j=0xfe;
	uchar t;
	TR0=0;
	TH0=0xfe;
	TL0=0x0c;
	timing=timing+1;
	if(timing==60)
	{
		hide=~hide;
		timing=0;
	}
	else if(timing==0)
	{
		hide=0xff;
	}
	if(k!=0xff&&i==k)
	{
		t=disp[i]&hide;
	}	
	else
	{
		t=disp[i];
	}
	OE=1;
	LE1=1;
	P0=table[t];
	LE1=0;
	LE2=1;
	P0=sled_bit[i];
	LE2=0;
	OE=0;
	i++;
	if(i==8)
	{
		i=0;
	}
	TR0=1;
}