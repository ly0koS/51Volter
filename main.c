/*
	param[6]=1--->Sin
	param[6]=2--->Square Wave
*/

#include "DISP.h"
#include "IIC.h"
#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

sbit recout=P1^0;

bit erflags;
bit setup;
bit selectwp;
bit rise;
bit setupend;

data uchar  param[9]={0x55,50,20,0,50,50,2};
data uchar 	k;
data uint 	para1,para2;
data uchar 	da;
data uchar 	so;

extern uchar disp[8];
extern uchar d2;

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
	Calculate();
	display(0,15);
	disp[1]=param[6];
	setup=0;
	while(1)
	{
		keynum=keyscan();
		if(setup==1)											//Setup Mode
		{
			if(keynum==12)								//setting
			{
				setup=0;
				if(setupend==1)
				{
					setupend=0;
					write24c02();
					Calculate();
					k=99;
				}
				disp[1]=param[6];
				display(0,15);
			}
			if(keynum==13)								//select wave
			{
				if(selectwp==1)
				{
					switch(param[6])
					{
						case(1):param[6]=2;break;
						case(2):param[6]=1;break;
					}
					display(5,15);
				}
				else
				{
					selectwp=1;
					k=1;
					disp[1]=1;
				}
				display(5,15);
			}
			if(keynum==14)								//select param
			{
				k=2;
				if(selectwp==0)
				{
					if(disp[2]!=15)
						display(5,15);
					else
						display(5,10);
				}
				else
					selectwp=0;
			}
			if(keynum==110)								//go left
			{
				if(selectwp==0)
				{
					if(disp[2]==15&&disp[1]==2)
					{
						if(k>4)
							k--;
					}
					else if(k>6)
						k--;
				}
			}
			if(keynum==120)								//go right
			{
				if(selectwp==0)
				{
					if(disp[2]==15&&disp[1]==2)
					{
						if(k<=4)
							k=4;
						else if(disp[4]==1)
							k=4;
					}
					else if(k<=6)
						k=6;
					else if(param[6]==1&&disp[2]==10&&disp[6]==5)
						k=6;
					else
						k++;
				}
			}
			if(keynum==15)								//confim
			{
				setupend=1;
				ParamProcess();
			}
			else if(keynum<=9&&keynum>=0)
			{
				NumProcess(keynum);
			}
		}
		else if(setup==0)													//Not in Setup Mode
		{
			if(keynum==12)
			{
				setup=1;
				disp[1]=param[6];
				display(5,15);
				selectwp=1; 
				k=1;
			}
			if(keynum==13)
			{
			  disp[1]=disp[1]+1; 
			  if(disp[1]>=3) 
					disp[1]=1;
			  display(0,15);
			}
			if(keynum==14)
			{ 						
			  if(disp[2]!=15) 
					display(0,15);
			  else    
					display(0,10);
			}
			if(keynum==15)	
			{
			if(disp[1]!=param[6])
			  {
			   	if(param[6]==1)	
						Stop();
					param[6]=disp[1];
					EA=0;
					write24c02();
					display(0,15);
			  	Calculate();
					EA=1;
			  }
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
			SendByte(0x91);
			cAck();	
			if(erflags==1) goto sen;

		SendByte(so);
		cAck();
	}
	else if(param[6]==2)
	{
//		Calculate();
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