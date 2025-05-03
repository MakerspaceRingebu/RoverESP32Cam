#pragma once

#include <Arduino.h>
#include "WebServer.hpp"
#include "ServoWrapper.hpp"

class ServoWebServer : public WebServer{
    public:
        ServoWebServer(int port, ServoWrapper servoA, ServoWrapper servoB);

        void UpdateServo();

    private:
        ServoWrapper _servoA;
        ServoWrapper _servoB;

        void HandleClient(WebClient * client);
        int SplitPath(const String& path, String* parts, int maxParts);
        void HandleCommand(WebClient * client, String servoName, String action, String angle);
};

ServoWebServer::ServoWebServer(int port, ServoWrapper servoA, ServoWrapper servoB) : WebServer{port}, _servoA(servoA), _servoB(servoB)
{

}

void ServoWebServer::HandleCommand(WebClient * client, String servoName, String action, String angle){
    ServoWrapper* selectedServo = nullptr;
    bool add;

    if (servoName == "a") {
        selectedServo = &_servoA;
    } else if (servoName == "b") {
        selectedServo = &_servoB;
    } else {
        client->SendBody("Unknown servo (expected A or B)");
        return;
    }

    if (action == "add"){
        add = true;
    } else if(action == "set"){
        add = false;
    }else{
        client->SendBody("Unknown command (expected add or set)");
        return;
    }

    int angleValue = angle.toInt();

    if(add){
        client->SendBody("Adding angle to servo");
    }else{
        client->SendBody("Setting servo");
    }

    
    if(add){
        selectedServo->AddAngle(angleValue);
    }else{
        selectedServo->SetAngle(angleValue);
    }
}

void ServoWebServer::HandleClient(WebClient * client){
    String requestedPage = client->GetRequestedPage();

    //URL format: /servo/{servoName}/{Set/Add}/{AngleDegrees}
    //Example request: /servo/A/Set/90
    if(requestedPage.startsWith("/servo/")){
        const int expectedParts = 4;
        String parts[expectedParts];
        int partCount = SplitPath(requestedPage, parts, expectedParts);

        if(partCount < expectedParts){
            client->SendBody("Too few parameters");
        }else{
            HandleCommand(client, parts[1], parts[2], parts[3]);
        }

    }else{
        client->SendBody("Unknown command");
    }
    client->Disconnect();
}

int ServoWebServer::SplitPath(const String& path, String* parts, int maxParts) {
    int partCount = 0;
    int start = 1; // skip the initial '/'
    while (start < path.length() && partCount < maxParts) {
        int end = path.indexOf('/', start);
        if (end == -1) end = path.length();
        parts[partCount++] = path.substring(start, end);
        start = end + 1;
    }
    return partCount;
}

void ServoWebServer::UpdateServo(){
    Update();
    
    for(int i = _clients.size() - 1 ; i >= 0 ; i--){
        HandleClient(_clients[i]);
    }
}