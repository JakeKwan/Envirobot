#pragma once

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "RoverControls.h"

// Global declarations
extern const char* ssid;
extern const char* password;
extern AsyncWebServer server;
extern AsyncWebSocket ws;

extern const int controlLedPin;
extern const int controlPinA;
extern const int controlPinB;

// Function declarations
void broadcastSerial(const String& message);
void setupControlPins();
void setControlState(bool active);
void processWebCommand(const String& command);
void onWsEvent(AsyncWebSocket* socket, AsyncWebSocketClient* client, AwsEventType type,
               void* arg, uint8_t* data, size_t len);

void setup_serial();
void handle_serial();