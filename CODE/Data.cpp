#include "Data.h"

//Data 的成员函数
Data::Data() : jumpCount(0), jumpSpeed(0), sampleCount(0) ,AcX(0), AcY(0), AcZ(0), GyX(0), GyY(0), GyZ(0) {}

void Data::update(int16_t newAcX, int16_t newAcY, int16_t newAcZ, int16_t newGyX, int16_t newGyY, int16_t newGyZ) {
    
    sampleCount++;

    AcX = newAcX;
    AcY = newAcY;
    AcZ = newAcZ;
    GyX = newGyX;
    GyY = newGyY;
    GyZ = newGyZ;

    AcXStats.update(AcX);
    AcYStats.update(AcY);
    AcZStats.update(AcZ);
    GyXStats.update(GyX);
    GyYStats.update(GyY);
    GyZStats.update(GyZ);
}


