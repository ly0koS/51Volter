#ifndef _I2C_
#define _I2C_
#include <intrins.h>
#include <reg52.h>

#define uchar unsigned char

void Start(void);                           //起始信号
void Stop(void);                            //终止信号
void Ack(void);                             //应答信号
void NoAck(void);                           //非应答信号
void cAck(void);
void SendByte(unsigned char send);
unsigned char RcvByte(void);
void delay4us(void);
void delay5us();

#endif