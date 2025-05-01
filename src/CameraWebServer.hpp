#pragma once

#include "WebServer.hpp"

class CameraWebServer : public WebServer{
    public:
        CameraWebServer(int port);
        void SendToAllClientsWithPage(String contentType, String requestedPage, char * body, int size);
};

CameraWebServer::CameraWebServer(int port) : WebServer{port}
{
    
}


void CameraWebServer::SendToAllClientsWithPage(String contentType, String requestedPage, char * body, int size){
    if(size < 0)
        return;

    for(int i = _clients.size() - 1 ; i >= 0 ; i--){
        if(_clients[i]->GetRequestedPage().equals(requestedPage)){
            Serial.print("Sending stream: ");
            Serial.println(size);

/*
            for(int i = 0 ; i < size ; i++){
                Serial.print(body[i]);
            }*/

            _clients[i]->SendStream(contentType, body, size);
        }
    }
}