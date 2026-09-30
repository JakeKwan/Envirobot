#include "StateMachine.h"

void StateMachine::sm_setup() {
    setup_serial(); 
    steering.begin(); 
    steering.servo_test(); 
    Serial.println("[StateMachine] setup complete");
}

void StateMachine::sm_loop() {
    // get controls from serial 

    switch(rover_state) 
    {
        default: 
            break; 
        case switcher: 
            break; 
        case driving: 
            RoverControlState control = getRoverControlState(); 
            steering.applyControl(control); 
            
            Serial.print(" throttle: "), Serial.print(control.throttle); 
            Serial.print(" steering: "), Serial.print(control.steering); 
            break; 
    }; 
    
    Serial.print("active state: "), Serial.println(rover_state); 
    handle_serial(); 
    delay(100);
}
