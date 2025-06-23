#ifndef CALORIECALCULATOR_H
#define CALORIECALCULATOR_H
/*
用于计算消耗的卡路里数量，唯一一个成员函数 calculateCalories，后续如果加其他的算法的话可以在类中扩展
去年这块没找到论文，所以这个计算方法是比较原始的，只是通过物理去实验的
今年如果有更好的算法的话可以考虑用重载的方法去替换这个函数！
*/
#include "Data.h"

class CalorieCalculator {
public:
    static double calculateCalories(const Data& data, unsigned long elapsedTime, int bodyType);
};

#endif // CALORIECALCULATOR_H
