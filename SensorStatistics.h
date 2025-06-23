#ifndef SENSORSTATISTICS_H
#define SENSORSTATISTICS_H

/**
 * @file SensorStatistics.h
 * @brief 传感器数据统计模板类，用于处理和分析各种类型的传感器数据
 * 
 * 这里直接是用模版类实现的多态，如果在 stream 里面想知道任何一个数据的三点特征信息的话，直接 new 一个可以直接套用进去
 * 详细的使用可以看 Data 类里面的定义，在里面 new 一下这个对象，就可以直接依赖于 template T 的封装，而不用全部继承
 * 我承认这块写的有点抽象，不懂的可以微信 dd 我
 * 
 * 该模板类提供了一个通用框架，可以处理不同数据类型的传感器读数，
 * 包括整型、浮点型等数值类型。它能够跟踪并计算传感器数据的最大值、
 * 最小值和平均值，适用于需要实时监控和分析传感器数据的应用场景。
 */

template <typename T>
class SensorStatistics {
public:
    SensorStatistics();

    void update(T value);
    T getMax() const;
    T getMin() const;
    double getAverage() const;

private:
    T maxVal;
    T minVal;
    T sum;
    size_t count;
};

template <typename T>
SensorStatistics<T>::SensorStatistics()
    : maxVal(static_cast<T>(0)), minVal(static_cast<T>(0)), sum(0), count(0) {
    maxVal = sizeof(T) == 1 ? 0 : -32768; 
    minVal = sizeof(T) == 1 ? 255 : 32767; 
}

template <typename T>
void SensorStatistics<T>::update(T value) {
    if (value > maxVal) {
        maxVal = value;
    }
    if (value < minVal) {
        minVal = value;
    }
    sum += value;
    count++;
}

template <typename T>
T SensorStatistics<T>::getMax() const {
    return maxVal;
}

template <typename T>
T SensorStatistics<T>::getMin() const {
    return minVal;
}

template <typename T>
double SensorStatistics<T>::getAverage() const {
    return count == 0 ? 0 : static_cast<double>(sum) / count;
}

#endif // SENSORSTATISTICS_H
