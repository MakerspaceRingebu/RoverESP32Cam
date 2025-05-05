#pragma once

#include <Arduino.h>

class ServoWrapper {
    public:
        ServoWrapper(int pin);
        void SetAngle(int angle);
        void AddAngle(int angle);
        void Update();
    
    private:
        int _pin;
        int _currentAngle = 90;

        const int _minAngle = 10;
        const int _maxAngle = 170;

        void WriteCurrentAngle();
};

ServoWrapper::ServoWrapper(int pin){
    _pin = pin;
    pinMode(_pin, OUTPUT);
    WriteCurrentAngle();
}

void ServoWrapper::SetAngle(int angle){
    if (angle < _minAngle) angle = _minAngle;
    if (angle > _maxAngle) angle = _maxAngle;
    _currentAngle = angle;

    WriteCurrentAngle();
    Serial.print("CURRENT ANGLE: ");
    Serial.println(_currentAngle, DEC);
}
void ServoWrapper::AddAngle(int angle){
    SetAngle(_currentAngle+angle);
}
void ServoWrapper::Update(){
}

void ServoWrapper::WriteCurrentAngle(){
    float percentage = ((float)_currentAngle) / 180.0f;
    int usHigh = 1000 + (int)(percentage * 1000.0f);

    for(int i = 0 ; i < 10 ; i++){
        digitalWrite(_pin, HIGH);
        delayMicroseconds(usHigh);        
        digitalWrite(_pin, LOW);
        delayMicroseconds(20000);
    }
}

