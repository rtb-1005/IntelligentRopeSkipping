#include "SmartJumpRope.h"
#include "LCDUI.h"
#include "Joystick.h"
#include "calculator.h"

const int joystickXPin = A0;
const int joystickYPin = A1;
const int joystickButtonPin = 7;

SmartJumpRope smartRope(12, 11, 5, 4, 3, 2); // 传递液晶显示器的引脚
LCDUI lcd(12, 11, 5, 4, 3, 2);
Joystick joystick(joystickXPin, joystickYPin, joystickButtonPin);


//状态机的定义，这里很重要，不过并没有全部 while1，也不是底层状态机，而是一个顺序结构嵌套了一下。因为 switch case 遭遇了很大的错误，可能怀疑是转换的函数没有写好
//这里 debug 了 3 个小时，建议不要动下面所有的代码！！！
//不清楚在 loop 里面直接用continue行不行，可以考虑while1 嵌套加结算的模式，后面直接进循环尾部可以直接从头初始化
//所有的初始化都做了两次，也就是说所有函数的析构都写在了最后的区域，所以后续跳绳的时候应该不会有问题
//可以考虑加入 tf卡，导入 json 库，做一些信号处理的暂存（需要独自处理 buffer 的 I/O）

enum State {
    WELCOME,
    WEIGHT_SELECTION,
    JUMP_ROPE,
    JUMP_END
};

State currentState = WELCOME;
int weightSelection = 0; // 0 = S, 1 = M, 2 = L

void setup() {
    lcd.initialize();
    joystick.initialize();
    Serial.begin(9600); // 用于调试信息输出
}

void loop() {
    bool readyToJump = false;

    while (!readyToJump) {
        Serial.print("Current state: ");
        Serial.println(currentState);

        switch (currentState) {
            case WELCOME:
                lcd.displayWelcome();
                delay(2000); // 显示欢迎信息两秒
                currentState = WEIGHT_SELECTION;
                lcd.displayWeightSelection(weightSelection);
                Serial.println("State: WEIGHT_SELECTION");
                break;

            case WEIGHT_SELECTION:
                int yValue = joystick.readY();
                if (yValue > 800) { // 向下移动摇杆
                    weightSelection = min(weightSelection + 1, 2);
                    lcd.displayWeightSelection(weightSelection);
                    delay(300); // 防止抖动
                } else if (yValue < 200) { // 向上移动摇杆
                    weightSelection = max(weightSelection - 1, 0);
                    lcd.displayWeightSelection(weightSelection);
                    delay(300); // 防止抖动
                }
                if (joystick.isButtonPressed()) { // 按下按钮确认
                    currentState = JUMP_ROPE;
                    smartRope.initialize(); // 重新初始化跳绳，重置计数等
                    Serial.println("State: JUMP_ROPE");
                    readyToJump = true;
                    delay(300); // 防止抖动
                }
                break;
        }
    }

    while (true) {
        smartRope.update();
        // 在设定的时间间隔后更新显示
        static unsigned long lastDisplayUpdateTime = 0;
        const unsigned long displayUpdateInterval = 500; // 显示更新间隔（毫秒）

        if (millis() - lastDisplayUpdateTime > displayUpdateInterval) {
            lcd.displayData(smartRope.getJumpCount(), smartRope.getElapsedTime());
            lastDisplayUpdateTime = millis();
        }

        // 检查是否按下按钮以停止跳绳
        if (joystick.isButtonPressed()) {
            break; // 退出跳绳循环，结束跳绳
        }
    }

    // 跳绳结束后的显示逻辑
    int jumpCount = smartRope.getJumpCount();
    unsigned long elapsedTime = smartRope.getElapsedTime();
    double calories = CalorieCalculator::calculateCalories(smartRope.getData(), elapsedTime, weightSelection);
    int bpm = (jumpCount * 60000) / elapsedTime;
    
    while (true) {
        lcd.displayEnd(jumpCount, elapsedTime, calories, bpm);
        Serial.print("Calories burned: ");
        Serial.println(calories);
        Serial.print("BPM: ");
        Serial.println(bpm);
        Serial.println("State: JUMP_END");
        delay(300); // 防止抖动
        if (joystick.isButtonPressed()) {
            break; // 退出跳绳循环，结束跳绳
    }
}
}
