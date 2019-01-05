/*
	param[6]=1--->Sin
	param[6]=2--->Square Wave
*/

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

data uchar  param[9]={0x55,50,50,3,232,50,2};
data uchar  p;
data uchar  hide;
data uchar 	k;
data uint 	para1,para2;
data uchar 	da;
data uchar 	so;

extern uchar disp[8];
extern d2;

int main()
{
	int keynum;
	EA=1;
	ET0=1;
	ET1=1;
	PT0=1;
	PT1=0;
	TH0=(65536-500)/256;
	TL0=(65536-500)%256;
	TR0=1;
	TR1=1;
	read24c02();
	display(0,15);
	while(1)
	{
		keynum=keyscan();
		if(keynum==12)
		{
			if(setup==1)
			{
				if(setupend==1)
				{
					write24c02;
					Calculate();
					setupend=0;
					display(0,15);
					k=99;														//no blink
				}
				setup=0;
			}
			else
			{
				setup=1;
				disp[1]=param[6];
				display(5,15);
				selectwp=1;
				k=1;
			}
		}
		if(keynum==13)
		{
			if(selectwp==1)
			{
				switch(param[6])
				{
					case(1):param[6]=2;break;
					case(2):param[6]=1;break;
				}
			}
			else
			{
				k=1;
				disp[k]=1;
				display(5,15);
			}
		}
		if(keynum==14)
		{
			if(setup==1)
			{
				if(d2!=15)
					d2=15;
				else
					d2=10;
				if(keynum==110)
				{
					if(param[6]==1&&k>6)
						k=k-1;
					else if(param[6]==2&&k>4)											//What if in Vpp mode?
						k=k-1;
				}
				if(keynum==120)
				{
					if(param[6]==1)
					{
						if(k<6)
							k=6;
						else if(disp[6]==5)
							k=6;
						else if(k==7)
							k=6;
						else
							k=k+1;
					}
					else if(param[6]==2)													//What if in Vpp mode?
					{
						if(k<4)
							k=4;
						else if(disp[4]==1)
							k=4;
						else if(k==7)
							k=4;
						else
							k=k+1;
					}
				}
				if(keynum>=0&&keynum<=8)
				{
					NumProcess(keynum);
				}
				if(keynum==16)
				{
					setupend=1;
					ParamProcess();
				}
			}
			else
			{
				setup=1;
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