#include "I2C.h"

#define uchar unsigned char

sbit sda=P3^4;
sbit scl=P3^6;
sbit erflags=PSW^5;
sbit Cy=PSW^7;

void Start(void)
{
	scl=1;
	sda=1;
	delay5us();
	sda=0;
	delay4us();
	scl=0;
}

void Stop(void)
{
	scl=0;
	sda=0;
	delay4us();
	scl=1;
	delay4us();
	sda=1;
	delay5us();
	sda=0;
}

void Ack(void)
{
	uchar i;
	sda=0;
	scl=1;
	delay4us();
	while((sda==1)&&(i<255))i++;
	scl=0;
	delay4us();
}
	
void NoAck(void)
{
	sda=1;
	scl=1;
	delay4us();
	scl=0;
	sda=0;
}

void cAck(void)
{
	sda=1;
	scl=1;
	F0=0;
	if(sda==1)
		F0=1;
	scl=0;
	_nop_();
}

void SendByte(uchar send)
{
	uchar i,temp;
	temp=send;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
		delay4us();
		sda=Cy;
		delay4us();
		scl=1;
		delay4us();
	}
	scl=0;
	delay4us();
	sda=1;
	delay4us();
}
	
uchar RcvByte(void)
{
	uchar i,temp;
	scl=0;
	delay4us();
	sda=1;
	for(i=0;i<8;i++)
	{
		scl=1;
		delay4us();
		temp=(temp<<1)|sda;
		scl=0;
		delay4us();
	}
	delay4us();
			return temp;
}

void delay4us(void)
{
	_nop_();_nop_();_nop_();_nop_();
}
void delay5us()
{
	_nop_();_nop_();_nop_();_nop_();_nop_();
}