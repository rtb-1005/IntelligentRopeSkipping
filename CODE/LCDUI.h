#ifndef LCDUI_H
#define LCDUI_H


//注释被我写进cpp 文件去了，这块就不写了，那里看的更明白一些。

#include <LiquidCrystal.h>

class LCDUI {
public:
    LCDUI(int rs, int en, int d4, int d5, int d6, int d7);
    void initialize();
    void displayWelcome();
    void displayWeightSelection(int selection);
    void displayCountdown();
    void displayData(int jumpCount, unsigned long elapsedTime);
    void displayEnd(int jumpCount, unsigned long elapsedTime, double calories, int bpm);
    void clear();


private:
    LiquidCrystal lcd;
    void scrollText(const String& text, int row, int delayTime);
};

#endif // LCDUI_H
