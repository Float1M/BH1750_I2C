#ifndef BH1750_H
#define BH1750_H

#include "stm32f10x.h"
#include "app_i2c.h"
#include "define.h"

void BH1750_Init(void);
float BH1750_Read_Lux(void);

#endif
