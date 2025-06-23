#include "LCDUI.h"
#include <Arduino.h>

//构造函数的时候默认引入引脚定义，一般都是在 main.ino里面直接进行 new 的操作，引脚在 setup 里宏观调控

LCDUI::LCDUI(int rs, int en, int d4, int d5, int d6, int d7) 
    : lcd(rs, en, d4, d5, d6, d7) {}

void LCDUI::initialize() {
    lcd.begin(16, 2);
    displayWelcome(); //setuo 里可以直接实现欢迎界面
}

void LCDUI::clear() {  //内部直接封装，禁止外部直接调用 clear
    lcd.clear();
}

void LCDUI::displayWelcome() { //init 的第二段
    lcd.clear(); 
    lcd.print("Smart Jump Rope");
    delay(2000);  // 显示启动信息两秒
    lcd.clear();
    lcd.print("Choose ur weight:");
    displayWeightSelection(0); // 初始化选择位置
}

//下面是一些渲染模组，负责菜单栏的渲染形式，已经调整好了，保持一致性
//所有的渲染模组都对应状态机的状态，请仔细区分！！！！

void LCDUI::displayWeightSelection(int selection) { //体重选择的部分
    lcd.setCursor(0, 1);
    lcd.print("S  M  L");
    lcd.setCursor(selection * 3, 1); // 设置光标位置高亮选中
    lcd.print('>');
}

void LCDUI::displayCountdown(){ //准备跳绳前摇的部分，倒计时
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jump Rope Start");
  lcd.setCursor(0, 1);
  lcd.print("in 3");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jump Rope Start");
  lcd.setCursor(0, 1);
  lcd.print("in 2");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jump Rope Start");
  lcd.setCursor(0, 1);
  lcd.print("in 1");
  delay(1000);
}

void LCDUI::displayData(int jumpCount, unsigned long elapsedTime) { //跳绳实时监测部分
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Jumps: ");
    lcd.print(jumpCount);
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(elapsedTime / 1000);
    lcd.print("s");
}

void LCDUI::displayEnd(int jumpCount, unsigned long elapsedTime, double calories, int bpm) { //结束清算部分
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sport Result:");
    
    lcd.setCursor(0, 1);
    lcd.print("jumps: ");
    lcd.print(jumpCount);
    delay(1000);
    
    


    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sport Result:");
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(elapsedTime / 1000);
    lcd.print("s");
    delay(1000);


    
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sport Result:");
    lcd.setCursor(0, 1);
    lcd.print("Calories: ");
    lcd.print(calories);
    delay(1000);


    
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sport Result:");
    lcd.setCursor(0, 1);
    lcd.print("BPM: ");
    lcd.print(bpm);
    delay(1000);
}



