#ifndef DATA_H
#define DATA_H

//这个文件中定义了传递的基础数据包，对于特征数据的处理还是依赖 SensorStatistics.h。
#include <MPU6050.h>
#include "SensorStatistics.h"


class Data {
public:
    int jumpCount;
    float jumpSpeed;
    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
    unsigned long sampleCount;

    Data();

    void update(int16_t newAcX, int16_t newAcY, int16_t newAcZ, int16_t newGyX, int16_t newGyY, int16_t newGyZ);
    

    SensorStatistics<int16_t> AcXStats;
    SensorStatistics<int16_t> AcYStats;
    SensorStatistics<int16_t> AcZStats;
    SensorStatistics<int16_t> GyXStats;
    SensorStatistics<int16_t> GyYStats;
    SensorStatistics<int16_t> GyZStats;
};

#endif // DATA_H
