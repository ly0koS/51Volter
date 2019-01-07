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


bit erflags;
bit setup;
bit selectwp;
bit rise;
bit setupend;

data uchar  param[9]={0x55,25,50,0,50,50,1};
data uchar 	k;
data uint 	para1 _at_ 0x10;
data uint 	para2 _at_ 0x11;
data uchar 	da;
data uchar 	so;
data uint	keynum _at_ 0x50;

extern uchar disp[8];
extern uchar d2;
extern uchar hide;
extern uchar i;

int main()
{
	uchar a,b;
	EA=1;
	ET0=1;
	ET1=1;
	TMOD=0x11;
	TH0=(65536-500)/256;
	TL0=(65536-500)%256;
	TR0=1;
	TR1=1;
	read24c02();
	Calculate();
	disp[1]=param[6];
	display(0,15);
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
		else
			keynum=255;
		if(setup==1)											//Setup Mode
		{
			if(keynum==12)								//setting
			{
				setup=0;
				if(setupend==1)
				{
					setupend=0;
					EA=0;
					write24c02();
					Calculate();
					EA=1;
					k=255;
				}
				disp[1]=param[6];
				display(0,15);
				keynum=255;
			}
			if(keynum==13)								//select wave
			{
				if(selectwp==1)
				{
					switch(param[6])
					{
						case(1):disp[1]=2;break;
						case(2):disp[1]=1;break;
					}
				}
				else
				{
					selectwp=1;
					k=1;
					disp[1]=1;
				}
				display(5,15);
				keynum=255;
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
				keynum=255;
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
				keynum=255;
			}
			if(keynum==20)								//go right
			{
				if(selectwp==0)
				{
					if(k<7)
						k++;
					if((disp[2]==15)&&(disp[1]==2))
						{
							if((k<4)||(disp[4]==1))
								k=4;
						}
						else 
						{
							if(k<6)
								k=6;
							if ((disp[2]!=13)&&(disp[6]==5))
								k=6;
							if ((disp[2]==13)&&(disp[6]==8))
								k=6;
						}			
				}
				keynum=255;
			}
			if(keynum==15)								//confim
			{
				setupend=1;
				ParamProcess();
				keynum=255;
			}
			else if(keynum<=9&&keynum>=0)
			{
				NumProcess(keynum);
				keynum=255;
			}
		}
		else 													//Not in Setup Mode
		{
			k=255;
			if(keynum==12)
			{
				setup=1;
				disp[1]=param[6];
				display(5,15);
				selectwp=1; 
				k=1;
				keynum=255;
			}
			if(keynum==13)
			{
			  disp[1]=disp[1]+1; 
			  if(disp[1]>=3) 
					disp[1]=1;
			  display(0,15);
				keynum=255;
			}
			if(keynum==14)
			{ 						
			  if(disp[2]!=15) 
					display(0,15);
			  else    
					display(0,10);
				keynum=255;
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
				keynum=255;
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
			if(da<=239)	da=da+16;
			else {rise=0; da=255;}
		}
		else 
		{ 	
			if(da>=17)	da=da-16;
			else {rise=1; da=0;}
		}
		so=para2*da/50;
		/*Add I2C DAC Code*/
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

