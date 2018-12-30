#include "DISP.h"

sbit OE=P1^3;
sbit LE1=P1^4;
sbit LE2=P1^5;
sbit K1=P2^0;
sbit K2=P2^1;


#define uchar unsigned char
	
extern uchar disp[8]={16,16,16,16,16,16,16,16};
uchar code sled_bit[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar table[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
uchar i=0;
uchar key=99;
uchar temp;

extern data uchar  param[9];

void display(uchar d1,d2)
{
	disp[0]=d1;
	disp[1]=param[6];
	if(d2==0x0f)
	{
		switch(param[6])
		{
			case(1):
			{
				disp[7]=param[1]%10;
				disp[6]=param[1]/10;
				break;
			}
			case(2):
			{
				disp[4]=param[3];
				disp[3]=param[4]/100;
				disp[2]=(param[4]/10)%10;
				disp[1]=param[4]%10;
				break;
			}
		}
	}
	else if(d2==0x0a)
	{
		switch(param[6])
		{
			case(1):
			{
				disp[7]=param[2]%10;
				disp[6]=param[2]/10;
				break;
			}
			case(2):
			{
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
	uchar k;
	TH0=0xfe;
	TL0=0x0c;
	k=disp[i];
	OE=1;
	LE1=1;
	P0=table[k];
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
}