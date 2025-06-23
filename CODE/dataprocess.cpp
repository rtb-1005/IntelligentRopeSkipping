#include "dataprocess.h"
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>

#define WINDOW_SIZE 10  // 滑动窗口大小
#define GRAVITY 9.80665


DataProcess::DataProcess() : jumpCount(0), jumpSpeed(0.0), AcX(0), AcY(0), AcZ(0), GyX(0), GyY(0), GyZ(0), sampleCount(0), firstTriggerTime(0), windowIndex(0), windowFull(false) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
        accWindow[i] = 0.0;
    }
}


/**
 * @brief 计算滑动窗口内加速度数据的方差和平均值
 * 
 * @param mean 引用参数，用于返回计算得到的平均值
 * @return double 计算得到的方差值
 * 
 * 该函数首先确定有效的窗口大小（如果窗口未满，则使用当前索引作为大小）。
 * 然后遍历窗口内的数据，过滤掉绝对值大于30的异常值，计算有效数据的总和。
 * 如果没有有效数据，返回0.0。
 * 计算平均值后，再次遍历窗口计算方差：每个有效数据点与平均值的差的平方和除以有效数据点数。
 * 方差反映了加速度数据的波动程度，是检测跳绳动作的重要指标。
 * 这个东西本来想直接调用 SensorStatistic.h，但是考虑到对象之内不好做解耦，重新在这里实现了整个的函数
 */


double DataProcess::calculateVarianceAndMean(double &mean) {
    double sum = 0.0;
    double variance = 0.0;
    int validCount = 0;

    int windowSize = windowFull ? WINDOW_SIZE : windowIndex;

    for (int i = 0; i < windowSize; i++) {
        if (abs(accWindow[i]) <= 30) { // 滤除大于30的值
            sum += accWindow[i];
            validCount++;
        }
    }
    if (validCount == 0) return 0.0;

    mean = sum / validCount;

    for (int i = 0; i < windowSize; i++) {
        if (abs(accWindow[i]) <= 30) {
            variance += (accWindow[i] - mean) * (accWindow[i] - mean);
        }
    }
    variance /= validCount;

    return variance;
}



// 实现检查阈值的函数
int DataProcess::checkThreshold(Data& data, double varianceThreshold, double meanThreshold) {
    // 获取Z方向变换后的加速度
    double acc = getTransformedAccelerationZ(data);

    // 将加速度值添加到滑动窗口
    accWindow[windowIndex] = acc;
    windowIndex = (windowIndex + 1) % WINDOW_SIZE;
    if (windowIndex == 0) {
        windowFull = true;
    }

    // 计算滑动窗口内的方差和平均值
    double mean = 0.0;
    double variance = calculateVarianceAndMean(mean);

    // 如果方差大于阈值且平均值小于阈值，则返回1，否则返回0
    if (variance >= varianceThreshold && mean < meanThreshold) {
        if (firstTriggerTime == 0) {
            firstTriggerTime = millis(); 
        }
        return 1;
    }
    return 0;
}

// 计算欧拉角
void DataProcess::computeEulerAngles(float &roll, float &pitch, float &yaw) {
    roll = atan2(AcY, AcZ) * 180 / PI;
    pitch = atan2(-AcX, sqrt(AcY * AcY + AcZ * AcZ)) * 180 / PI;
    yaw = atan2(GyY, GyX) * 180 / PI; 
}

// 应用欧拉角进行坐标变换
void DataProcess::transformAcceleration(float roll, float pitch, float yaw, float &transformedAcX, float &transformedAcY, float &transformedAcZ) {
    float rollRad = roll * PI / 180;
    float pitchRad = pitch * PI / 180;
    float yawRad = yaw * PI / 180;

    float cosRoll = cos(rollRad);
    float sinRoll = sin(rollRad);
    float cosPitch = cos(pitchRad);
    float sinPitch = sin(pitchRad);
    float cosYaw = cos(yawRad);
    float sinYaw = sin(yawRad);

    // 转换后的加速度值
    transformedAcX = AcX * cosPitch * cosYaw + AcY * (cosYaw * sinPitch * sinRoll - sinYaw * cosRoll) + AcZ * (cosYaw * sinPitch * cosRoll + sinYaw * sinRoll);
    transformedAcY = AcX * cosPitch * sinYaw + AcY * (sinYaw * sinPitch * sinRoll + cosYaw * cosRoll) + AcZ * (sinYaw * sinPitch * cosRoll - cosYaw * sinRoll);
    transformedAcZ = -AcX * sinPitch + AcY * cosPitch * sinRoll + AcZ * cosPitch * cosRoll;
}

// 获取变换后的Z方向加速度
float DataProcess::getTransformedAccelerationZ(Data& data) {
    AcX = data.AcX;
    AcY = data.AcY;
    AcZ = data.AcZ;
    GyX = data.GyX;
    GyY = data.GyY;
    GyZ = data.GyZ;

    float roll, pitch, yaw;
    computeEulerAngles(roll, pitch, yaw);

    float transformedAcX, transformedAcY, transformedAcZ;
    transformAcceleration(roll, pitch, yaw, transformedAcX, transformedAcY, transformedAcZ);
    
    // 转换为 m/s^2
    float transformedAccZ = transformedAcZ * GRAVITY / 16384.0; // 假设加速度计的分辨率为 16384 LSB/g
    Serial.println(transformedAccZ);
    return transformedAccZ;
}

// 实现获取时间差的函数
bool DataProcess::getTimeDifference() {
    if (firstTriggerTime != 0) {
        unsigned long currentTime = millis();
        unsigned long diff = currentTime - firstTriggerTime;
        if (diff > 500) { // 0.6秒
            firstTriggerTime = 0; // 重置firstTriggerTime
            return true;
        }
    }
    return false; 
}

// 检查更新函数
int DataProcess::isUpdate(Data& data, double varianceThreshold, double meanThreshold) {
    return (checkThreshold(data, varianceThreshold, meanThreshold) && getTimeDifference());
}
