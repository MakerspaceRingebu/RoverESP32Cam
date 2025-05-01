#pragma once

#include <Arduino.h>
#include "WebServer.hpp"
#include "Servo.hpp"

class ServoWebServer : public WebServer{
    public:
        ServoWebServer(int port, Servo servoA, Servo servoB);

        void Update();

    private:
        Servo _servoA;
        Servo _servoB;

        void HandleClient(WebClient * client);
};

ServoWebServer::ServoWebServer(int port, Servo servoA, Servo servoB) : WebServer{port}, _servoA(servoA), _servoB(servoB)
{

}

void ServoWebServer::HandleClient(WebClient * client){
    String requestedPage = client->GetRequestedPage();

    if(requestedPage.startsWith("/servo/")){
        client->SendBody("Servo!");
    }else{
        client->SendBody("Unknown command");
    }
    client->Disconnect();
}


void ServoWebServer::Update(){
    WebServer::Update();
    
    for(int i = _clients.size() - 1 ; i >= 0 ; i--){
        HandleClient(_clients[i]);
    }
}