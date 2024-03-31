#pragma once

#include <Arduino.h>
#include "esp_camera.h"
#include "GlobalVars.hpp"


static camera_config_t camera_config = {
    .pin_pwdn  = 32,
    .pin_reset = -1,
    .pin_xclk = 0,
    .pin_sccb_sda = 26,
    .pin_sccb_scl = 27,

    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 21,
    .pin_d2 = 19,
    .pin_d1 = 18,
    .pin_d0 = 5,
    .pin_vsync = 25,
    .pin_href = 23,
    .pin_pclk = 22,

    .xclk_freq_hz = 16000000,//EXPERIMENTAL: Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,//YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_XGA,//QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.
    //.frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12, //0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1, //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY//CAMERA_GRAB_LATEST. Sets when buffers should be filled
};

bool camera_init(){
    //power up the camera if PWDN pin is defined
    if(camera_config.pin_pwdn != -1){
        pinMode(camera_config.pin_pwdn, OUTPUT);
        digitalWrite(camera_config.pin_pwdn, LOW);
    }

    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    GlobalVars::CameraErrorCode = err;
    if (err != ESP_OK) {
        Serial.println("Camera Init Failed");
        return false;
    }

    return true;
}


size_t camera_capture(uint8_t *& buf){
    //acquire a frame
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera Capture Failed");
        return -1;
    }
    buf = new uint8_t[fb->len];
    memcpy(buf, fb->buf, fb->len);
    esp_camera_fb_return(fb);   
    return fb->len;
}



class CameraReader{
    public:
        CameraReader();
        bool HasNewFrame();
        int GetFrame(char *& result);

        bool IsRunning();
    private:
        long _targetTimePerFrame = 100;
        long _lastTime;
        bool _isRunning = false;
};


CameraReader::CameraReader(){
    GlobalVars::CameraInitSuccess = false;
    if(camera_init()){
        _isRunning = true;
        GlobalVars::CameraInitSuccess = true;
    }
    
    _lastTime = millis();
}

bool CameraReader::IsRunning(){
    return _isRunning;
}

bool CameraReader::HasNewFrame(){
    if(!IsRunning())
        return false;

    return _lastTime + _targetTimePerFrame < millis();
}

int CameraReader::GetFrame(char *& result){
    _lastTime = millis();

    unsigned long start = micros();
    int length = camera_capture((uint8_t*&)result);    
    GlobalVars::TimeToGetFrame = micros()-start;

    return length;        
}