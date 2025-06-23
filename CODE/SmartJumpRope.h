#ifndef SMARTJUMPROPE_H
#define SMARTJUMPROPE_H

#include <MPU6050.h>
#include <LiquidCrystal.h>
#include "Data.h"
#include"dataprocess.h"


//最底层的架构，整个跳绳包括 Data 包的内嵌，都嵌套在这个 class 里面了，包含了程序运行的所有操作
//包含了所有的数据结构定义，代码的最底层架构。


class SmartJumpRope {
public:
    SmartJumpRope(int rs, int en, int d4, int d5, int d6, int d7);

    void initialize();
    void update();

    int getJumpCount() const;
    unsigned long getElapsedTime() const;
    Data getData() const;

private:
    MPU6050 mpu;
    LiquidCrystal lcd;
    long jumpCount;
    unsigned long startTime;
    unsigned long elapsedTime;
    Data data;
    DataProcess dp;

    void updateSensorData();
};

#endif // SMARTJUMPROPE_H
