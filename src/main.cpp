#include "StateMachine.h"

StateMachine *state_machine; 

void setup() 
{
  state_machine = new StateMachine();
  state_machine->sm_setup(); 
}; 

void loop() {
  state_machine->sm_loop(); 
}
// #include "WebSerialMonitor.h"
// #include "AnalogSensor.h"
// #include "RoverControls.h"
// #include "Steering.h"

// AnalogSensor soilSensor = AnalogSensor(5, "soil");
// Steering steering;

// void setup() {
//   resetRoverControlState();
//   steering.begin();
//   setupSerial();
//   steering.servo_test(); 
// }

// void loop() {
//   // soilSensor.update();
//   handleSerial();

//   RoverControlState controls = getRoverControlState();
//   steering.applyControl(controls);

//   if (controls.enabled) {
//     Serial.print("[ROVER] throttle=");
//     Serial.print(controls.throttle, 2);
//     Serial.print(" steering=");
//     Serial.println(controls.steering, 2);
//   }

//   delay(100);
// }