#include "Joystick.h"
#include <Arduino.h>

Joystick::Joystick(int pinX, int pinY, int pinButton)
    : pinX(pinX), pinY(pinY), pinButton(pinButton) {}

void Joystick::initialize() {
    pinMode(pinButton, INPUT_PULLUP);
}

int Joystick::readX() {
    return analogRead(pinX);
}

int Joystick::readY() {
    return analogRead(pinY);
}

bool Joystick::isButtonPressed() {
    return digitalRead(pinButton) == LOW; // 确保按钮按下时返回 true
}
