#include "calculator.h"

/*
成员函数的计算方法，是跳绳数量，还有体型的系数，力度因数还有时间因数的乘积。
*/
double CalorieCalculator::calculateCalories(const Data& data, unsigned long elapsedTime, int bodyType) {
    double baseCaloriesPerJump = 0.0;

    switch (bodyType) {
        case 0: // 小体型
            baseCaloriesPerJump = 0.05;
            break;
        case 1: // 中体型
            baseCaloriesPerJump = 0.07;
            break;
        case 2: // 大体型
            baseCaloriesPerJump = 0.1;
            break;
        default:
            baseCaloriesPerJump = 0.0;
            break;
    }

    //下面这块是做一些系数测算，这些测算用的是平均值，然后用最大最小的极端值做一个三点取样
    //如何得到各类的值，考虑到开发板可怜的内存问题，详情见 SensorStatistic.h 这个是很重要的最大最小平均值的测算函数，极致优化了空间复杂度。

    
    double averageForce = (data.AcXStats.getAverage() + data.AcYStats.getAverage() + data.AcZStats.getAverage()) / 3.0;
    double maxForce = (data.AcXStats.getMax() + data.AcYStats.getMax() + data.AcZStats.getMax()) / 3.0;
    double minForce = (data.AcXStats.getMin() + data.AcYStats.getMin() + data.AcZStats.getMin()) / 3.0;

    double forceFactor = (averageForce + maxForce - minForce) / 10000.0; // 根据传感器数据标准化大致的一个因子，去年这块没找到论文，如果有论文的话可以修改算法
    double timeFactor = elapsedTime / 6000.0; // 以分钟为单位的时间因子

    double cal = data.jumpCount * baseCaloriesPerJump * forceFactor * timeFactor;
    if (cal > 0)
      return cal;
    return 0;
}
