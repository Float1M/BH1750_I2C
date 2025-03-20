#include "bh1750.h"

int main(void) {
    I2C_Config();
    BH1750_Init();
    
    while (1) {
        float lux = BH1750_Read_Lux();
        // X? lý d? li?u lux
    }
}
