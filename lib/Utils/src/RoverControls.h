#pragma once

#include <Arduino.h>

struct RoverControlState {
  float throttle = 0.0f;
  float steering = 0.0f;
  bool enabled = false;
};

enum Buttons {
  x, 
  circle, 
  triangle, 
  square, 
  up, 
  down, 
  left, 
  right,  
  rb, 
  rt, 
  lb, 
  lt
  
}; 

struct GamepadInputState {
  static const int kButtonCount = 16;
  static const int kAxisCount = 8;

  bool buttons[kButtonCount] = {false};
  float axes[kAxisCount] = {0.0f};
  bool connected = false;
};

void resetRoverControlState();
void setRoverEnabled(bool enabled);
void updateRoverControlFromButton(int buttonIndex, bool pressed);
void updateRoverControlFromAxis(int axisIndex, float value);
RoverControlState getRoverControlState();
