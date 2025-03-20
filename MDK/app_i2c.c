#include "app_i2c.h"
#define I2C_TIMEOUT 10000


void I2C_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x0B;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 200000;
    I2C_Init(I2C1, &I2C_InitStructure);
    
    I2C_Cmd(I2C1, ENABLE);
}

int I2C_WaitForEvent(uint32_t event) {
    uint32_t ticks = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, event) && ticks) {
        ticks--;
    }
    return (ticks > 0); // Tra ve 1 neu thành công, 0 neu timeout
}

void I2C1_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    if (!I2C_WaitForEvent(I2C_EVENT_MASTER_MODE_SELECT)) return;

    I2C_Send7bitAddress(I2C1, dev_addr, I2C_Direction_Transmitter);
    if (!I2C_WaitForEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) return;

    I2C_SendData(I2C1, reg_addr);
    if (!I2C_WaitForEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) return;

    I2C_SendData(I2C1, data);
    if (!I2C_WaitForEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) return;

    I2C_GenerateSTOP(I2C1, ENABLE);
}


uint8_t I2C1_Read(uint8_t dev_addr, uint8_t reg_addr) {
    uint8_t data;
    I2C1_Read_Buf(dev_addr, reg_addr, &data, 1);
    return data;
}

void I2C1_Read_Buf(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t size) {
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, dev_addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    I2C_SendData(I2C1, reg_addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, dev_addr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
	  uint8_t i;
    for (i = 0; i < size - 1; i++) {
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buf[i] = I2C_ReceiveData(I2C1);
    }
    
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    buf[size - 1] = I2C_ReceiveData(I2C1);
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
}
