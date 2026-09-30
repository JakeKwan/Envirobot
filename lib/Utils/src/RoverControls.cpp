#include "RoverControls.h"

namespace {
RoverControlState g_state;
GamepadInputState g_gamepad;
}

void resetRoverControlState() {
  g_state.throttle = 0.0f;
  g_state.steering = 0.0f;
  g_state.enabled = true;
}

void setRoverEnabled(bool enabled) {
  g_state.enabled = enabled;
}

void updateRoverControlFromButton(int buttonIndex, bool pressed) {
  if (buttonIndex == 0) { // x button 
    setRoverEnabled(!pressed);
  }
}

void updateRoverControlFromAxis(int axisIndex, float value) {
  if (axisIndex == 1) {
    g_state.throttle = constrain(value, -1.0f, 1.0f);
  } else if (axisIndex == 0) {
    g_state.steering = constrain(value, -1.0f, 1.0f);
  }
}

RoverControlState getRoverControlState() {
  return g_state;
}
