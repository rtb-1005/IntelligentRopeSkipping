#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <stdint.h>
#include "Data.h"

#define WINDOW_SIZE 10


/**
 * @class DataProcess
 * @brief 数据处理类，用于处理跳绳运动中的传感器数据
 * 
 * 该类负责处理来自MPU6050传感器的加速度和陀螺仪数据，
 * 通过坐标变换和数据分析算法检测跳绳动作，
 * 并计算相关运动参数如跳绳次数和速度。
 */

 
class DataProcess {
public:
    int jumpCount;
    float jumpSpeed;
    float AcX, AcY, AcZ;
    float GyX, GyY, GyZ;
    unsigned long sampleCount;
    unsigned long firstTriggerTime;

    DataProcess();
    int checkThreshold(Data& data, double varianceThreshold, double meanThreshold);
    bool getTimeDifference();
    int isUpdate(Data& data, double varianceThreshold, double meanThreshold);
    void computeEulerAngles(float &roll, float &pitch, float &yaw);
    void transformAcceleration(float roll, float pitch, float yaw, float &transformedAcX, float &transformedAcY, float &transformedAcZ);
    float getTransformedAccelerationZ(Data& data);

private:
    float accWindow[WINDOW_SIZE];
    int windowIndex;
    bool windowFull;
    double calculateVarianceAndMean(double &mean);
};

#endif // DATAPROCESS_H
