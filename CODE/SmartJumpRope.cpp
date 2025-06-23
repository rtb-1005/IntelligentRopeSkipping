#include "SmartJumpRope.h"
#include <Wire.h>
#include "LCDUI.h"
//修改这两个数值进行数据分析
#define THE1 10 //均值阈值
#define THE2 17//方差阈值

SmartJumpRope::SmartJumpRope(int rs, int en, int d4, int d5, int d6, int d7) 
    : lcd(rs, en, d4, d5, d6, d7), jumpCount(0), startTime(0), elapsedTime(0) {}

void SmartJumpRope::initialize() {
    LCDUI lcd1(12, 11, 5, 4, 3, 2);
    Wire.begin();
    mpu.initialize();
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd1.displayCountdown();
    lcd.clear();
    startTime = millis();
    jumpCount = 0;
    elapsedTime = 0;
    Serial.println("SmartJumpRope initialized");
}

void SmartJumpRope::update() {
    updateSensorData();
    elapsedTime = millis() - startTime;
    jumpCount = data.jumpCount;
    // Serial.print("Elapsed Time: ");
    // Serial.print(elapsedTime);
    // Serial.print(" ms, Jump Count: ");
    // Serial.println(jumpCount);
}

void SmartJumpRope::updateSensorData() {
    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
    mpu.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);
    data.update(AcX, AcY, AcZ, GyX, GyY, GyZ);
    data.jumpCount += dp.isUpdate(data,THE1,THE2);
}

int SmartJumpRope::getJumpCount() const {
    return jumpCount;
}

unsigned long SmartJumpRope::getElapsedTime() const {
    return elapsedTime;
}

Data SmartJumpRope::getData() const {
    return data;
}

