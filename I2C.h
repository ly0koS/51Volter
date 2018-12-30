#ifndef _I2C_
#define _I2C_
#include <intrins.h>
#include <reg52.h>

#define uchar unsigned char

void Start(void);
void Stop(void);
void Ack(void);
void Ack(void);
void NoAck(void);
void cAck(void);
void SendByte(unsigned char send);
unsigned char RcvByte(void);
void delay4us(void);
void delay5us();

#endif