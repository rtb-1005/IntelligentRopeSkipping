#ifndef JOYSTICK_H
#define JOYSTICK_H

//跳绳上有个游戏摇杆用于菜单栏的选择，分别有两个方向的和按下按钮的操作，用于菜单选择，
//因为这个是模拟信号得出的，所以在对应的 src 的函数实现里面要做好防抖，所以直接写成一个对象的形式了
//这块不用太动

class Joystick {
public:
    Joystick(int pinX, int pinY, int pinButton);
    void initialize();
    int readX();
    int readY();
    bool isButtonPressed();

private:
    int pinX;
    int pinY;
    int pinButton;
};

#endif // JOYSTICK_H
