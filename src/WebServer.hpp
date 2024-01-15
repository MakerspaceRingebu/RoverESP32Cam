#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <string>
#include <vector>
#include "WebClient.hpp"
#include "GlobalVars.hpp"

#define DEBUG
#define MAXCLIENTS 5

class WebServer{
    public:
        WebServer(int port);
        ~WebServer();
        void Update();

        void SendToAllClientsWithPage(String contentType, String requestedPage, char * body, int size);
    private:
        WiFiServer * _wifiServer;
        std::vector<WebClient*> _clients;

        void AcceptNewClients();
        void RemoveDeadClients();
        void HandleClients();
};

WebServer::WebServer(int port){
    _wifiServer = new WiFiServer(port);
    _wifiServer->begin();
}

WebServer::~WebServer(){
    delete _wifiServer;
}

void WebServer::Update(){ 
    AcceptNewClients();
    RemoveDeadClients();
    HandleClients();
}

void WebServer::SendToAllClientsWithPage(String contentType, String requestedPage, char * body, int size){
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

void WebServer::AcceptNewClients(){
    while(true){
        WiFiClient clientLocal = _wifiServer->available();
        if(!clientLocal){
            return;
        }

#ifdef DEBUG
        Serial.println("New client waiting");
        Serial.flush();
#endif

        if (_clients.size() >= MAXCLIENTS) {
#ifdef DEBUG
            Serial.println("Client limit reached");
            Serial.flush();
#endif

            clientLocal.stop();
            return;
        }

#ifdef DEBUG
        Serial.println("New client accepted");
        Serial.flush();
#endif

        WebClient * webClient = new WebClient(clientLocal);
        _clients.push_back(webClient);
        GlobalVars::TotalClientsConnected++;
        GlobalVars::CurrentClientsConnected++;
    }
}

void WebServer::RemoveDeadClients(){
    for(int i = _clients.size() - 1 ; i >= 0 ; i--){
        if(!_clients[i]->IsConnected()){

            #ifdef DEBUG
                    Serial.println("Client is dead, removing");
                    Serial.flush();
            #endif

            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            GlobalVars::CurrentClientsConnected--;
        }
    }
}

void WebServer::HandleClients(){
    for(int i = _clients.size() - 1 ; i >= 0 ; i--){
        _clients[i]->Update();
    }
}


/*
class WebServer{
    public:
        WebServer(int port);
        ~WebServer();
        void Update();
    private:
        WiFiServer * _wifiServer;
        WebClient * _webClient = nullptr;

        bool IsClientConnected();
        bool TryConnectClient();
        void HandleClient();
        void DisconnectClient();
};

WebServer::WebServer(int port){
    _wifiServer = new WiFiServer(port);
    _wifiServer->begin();
}

WebServer::~WebServer(){
    delete _wifiServer;
}

bool WebServer::IsClientConnected(){
    return _webClient != nullptr;
}

bool WebServer::TryConnectClient(){
    WiFiClient * client = _wifiServer->available();
    if(client){
        _webClient = new WebClient(client);
        _webClient->ReadRequest();
        return true;
    }
    return false;
}

void WebServer::DisconnectClient(){
    delete(_webClient);
    _webClient = nullptr;

#ifdef DEBUG
    Serial.println("Disconnected client");
#endif
}

void WebServer::HandleClient(){
#ifdef DEBUG
        Serial.println("Handling client");
#endif

    _webClient->ReadRequest();

    if(_webClient->RequestIsCompleted()){
        _webClient->SendResponse("<html><body><h1>Hello world!</h1></body></html>");
        DisconnectClient();
    }
}

void WebServer::Update(){ 
    if(IsClientConnected() || TryConnectClient()){
        HandleClient();
    }
}
*/