#include "bh1750.h"

void BH1750_Init(void) {
    I2C1_Write(BH1750_ADDRESS, 0x01, 0); // Power On
    I2C1_Write(BH1750_ADDRESS, 0x10, 0); // Set mode: Continuously H-Resolution
}

float BH1750_Read_Lux(void) {
    uint8_t tmp8[2];
    I2C1_Read_Buf(BH1750_ADDRESS, 0x00, tmp8, 2);
    return (float)((tmp8[0] << 8) | tmp8[1]) / 1.2;
}
