#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>

class ServoWrapper {
    public:
        ServoWrapper(int pin);
        void SetAngle(int angle);
        void AddAngle(int angle);
        void Update();
    
    private:
        Servo _servo;
        int _currentAngle = 90;

        const int _minAngle = 10;
        const int _maxAngle = 170;
};

ServoWrapper::ServoWrapper(int pin){
    _servo.attach(pin);
    _servo.write(_currentAngle);
}

void ServoWrapper::SetAngle(int angle){
    if (angle < _minAngle) angle = _minAngle;
    if (angle > _maxAngle) angle = _maxAngle;
    _currentAngle = angle;
    _servo.write(_currentAngle);
}
void ServoWrapper::AddAngle(int angle){
    SetAngle(_currentAngle+angle);
}
void ServoWrapper::Update(){
    
}