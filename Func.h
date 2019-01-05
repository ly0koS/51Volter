#ifndef _Func_
#define _Func_
#include "I2C.h"
#include "DISP.h"
#define uchar unsigned char
void Calculate();
void write24c02(void);
void read24c02(void);
int keyscan();
void ParamProcess();
void NumProcess(unsigned int keynum);
#endif