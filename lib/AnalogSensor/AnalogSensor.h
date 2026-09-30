#include <Arduino.h>
#include <string.h>

class AnalogSensor
{
    public: 
    AnalogSensor(uint8_t analogPin, String name) 
    {
        this->analogPin = analogPin;
        this->name = name;  
        pinMode(analogPin, INPUT); 

        Serial.print(this->name);
        Serial.println(" loaded");
    } 
    
    uint8_t analogPin = 0; 
    String name;  

    void update()
    {
        // test values (simulate analog range 0-1023)
        uint16_t reading = millis() % 1024; // test values
        Serial.print(this->name);
        Serial.print(":");
        Serial.println(reading);
    }
};
