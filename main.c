/*
	param[6]=1--->An Wave
	param[6]=2--->Square Wave
*/

#include "DISP.h"
#include "IIC.h"
#include "Func.h"
#define uchar unsigned char
#define uint unsigned int

sbit recout=P1^0;
sbit OE=P1^3;
sbit duan=P1^4;
sbit wei=P1^5;
sbit K1=P2^0;
sbit K2=P2^1;

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
data uchar	keynum _at_ 0x50;

extern uchar disp[8],table[];
extern uchar code sled_bit[];
extern uchar d2;
extern uchar hide;
extern unsigned int timing;
extern uchar i;

int main()
{
	uchar a,b;
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
	while(1)
	{
		P2=0xf0;
		delay(5);	
		a=P2;
		P2=0x0f;
		delay(5);
		b=P2;
		a=a|b;
		if(a!=0xff)
		{
			while(P2!=0x0f);
			switch(a)
			{
				case 0xee: keynum=0;break;
				case 0xde: keynum=1;break;
				case 0xbe: keynum=2;break;
				case 0x7e: keynum=3;break;
				case 0xed: keynum=4;break;
				case 0xdd: keynum=5;break;
				case 0xbd: keynum=6;break;
				case 0x7d: keynum=7;break;
				case 0xeb: keynum=10;break;//左移
				case 0xdb: keynum=8;break;
				case 0xbb: keynum=9;break;
				case 0x7b: keynum=20;break;//右移
				case 0xe7: keynum=12;break;//设置键
				case 0xd7: keynum=13;break;//选择波形
				case 0xb7: keynum=14;break;//选择参数
				case 0x77: keynum=15;	//确认键
				}
		}
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
			if(keynum==10)								//go left
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
			if(keynum==20)								//go right
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
		else 													//Not in Setup Mode
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

void scan() interrupt 1
{
	uchar t;
	TR0=0;
	TH0=(65536-500)/256;
	TL0=(65536-500)%256;
	hide=0xff;
	if(timing==60)
	{
		hide=~hide;
		timing=0;
	}
	t=disp[i];
	OE=1;
	if(setup&&(k==i))
	{
		P0=table[t]&hide;
	}
	else
	{
		P0=table[t];
	}
	duan=1;
	duan=0;
	P0=sled_bit[i];
	wei=1;
	wei=0;
	OE=0;
	i++;
	if(i>=8)
	{
		i=0;
	}
	timing=timing+1;
	TR0=1;
}