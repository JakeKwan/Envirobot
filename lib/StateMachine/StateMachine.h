#include <Arduino.h>
#include "RoverConfig.h"
#include "WebSerialMonitor.h"
#include "RoverControls.h"
#include "Steering.h"

class StateMachine {
public: 
    StateMachine() = default; 
    void sm_setup(); 
    void sm_loop(); 

    enum RoverState {
        switcher, // for debugging
        driving, // separate arm control? 
    }; 

    enum SteeringState {
        normal,
        spinning
    }; 

    RoverState rover_state = driving; 
    SteeringState steering_state = normal; 
    Steering steering;
}; 