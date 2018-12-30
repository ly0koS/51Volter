#include "DISP.h"

sbit OE=P1^3;
sbit LE1=P1^4;
sbit LE2=P1^5;
sbit K1=P2^0;
sbit K2=P2^1;


#define uchar unsigned char
	
extern uchar disp[8];
uchar code sled_bit[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar table[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
uchar i=0;
uchar key=99;
uchar temp;


void display(uchar d1,d2)
{
	if(d1==0x05)
	{
		
	}
	else if(d1==0x0f)
	{
		
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

void Ex0() interrupt 0
{
	
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