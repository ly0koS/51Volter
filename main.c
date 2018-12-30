#include "DISP.h"
#include "I2C.h"
#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

sbit OE=P1^3;
sbit LE1=P1^4;
sbit LE2=P1^5;
sbit K1=P2^0;
sbit K2=P2^1;
sbit recout=P1^0;

bit erflags;
bit setup;
bit selectwp;
bit rise;
bit setupend;

data uchar disp[8]={16,16,16,16,16,16,16,16};
data uchar  param[9];
data uchar  p;
data uchar  hide;
data uint para1,para2;

int main()
{
	EA=1;
	ET0=1;
	ET1=1;
	PT0=1;
	PT1=0;
	TH0=0xfe;
	TL0=0x0c;
	TR0=1;
	TR1=1;
	OE=0;
	
	while(1)
	{
		read24c02();
		
	}
	return 0;
}


void output(void) interrupt 3
{
	
}