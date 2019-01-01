#include "DISP.h"
#include "I2C.h"
#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

bit erflags;
bit setup;
bit selectwp;
bit rise;
bit setupend;

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
	while(1)
	{
		read24c02();
	}
	return 0;
}


void output(void) interrupt 3
{
	
	TR1=0;
	TH1=para1/256;
	TL1=para1%256;
	if(param[6]==1)
	{
		if(rise)
		{}
		else
		{}
	}
	TR1=1;
}