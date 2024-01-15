#pragma once

#include <Arduino.h>
#include <string>

namespace GlobalVars{
    unsigned long Uptime = 0;
    unsigned long TimeToGetFrame = 0;
    bool CameraInitSuccess = false;
    int CameraErrorCode = -1;
    uint TotalClientsConnected = 0;
    uint CurrentClientsConnected = 0;


    String StatsToHTML(){
        String result = "<h2>Statistics</h2>";
        result+="<table>";
        result+="<tr><td>Uptime</td><td>" + String( Uptime ) + "ms</td></tr>";
        result+="<tr><td>Camera</td><td>" + String(CameraInitSuccess?"Enabled":"Failed") + "</td></tr>";
        result+="<tr><td>Camera errorcode</td><td>" + String(CameraErrorCode) + "</td></tr>";
        result+="<tr><td>Time to get frame</td><td>" + String( TimeToGetFrame ) + "us</td></tr>";
        result+="<tr><td>Total Clients Connected</td><td>" + String( TotalClientsConnected ) + "</td></tr>";
        result+="<tr><td>Current Clients Connected</td><td>" + String( CurrentClientsConnected ) + "</td></tr>";
        result+="</table>";
        return result;
    }
};
