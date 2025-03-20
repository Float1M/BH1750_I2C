#ifndef APP_I2C_H
#define APP_I2C_H

#include "stm32f10x.h"

void I2C_Config(void);
void I2C1_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t I2C1_Read(uint8_t dev_addr, uint8_t reg_addr);
void I2C1_Read_Buf(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t size);

#endif
