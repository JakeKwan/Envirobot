#include <Arduino.h>

/*
Input1	Input2	Spinning Direction
----------------------------------
Low(0)	Low(0)	Motor OFF
High(1)	Low(0)	Forward
Low(0)	High(1)	Backward
High(1)	High(1)	Motor OFF

*/


class Motor {
    
    Motor(int en, int in1, int in2) {
        this->en = en; 
        this->in1 = in1; 
        this->in2 = in2; 

        pinMode(en, OUTPUT);  
        pinMode(in1, OUTPUT);  
        pinMode(in2, OUTPUT);  
    } 

    // Pins 
    int en; // Controls speed 
    int in1; // Direction
    int in2; // Directon 


};

// #include <Arduino.h>

// // // Motor A connections
// // int enA = 9;
// // int in1 = 8;
// // int in2 = 7;
// // Motor B connections
// int enB = 27;
// int in3 = 12;
// int in4 = 14;




// // This function lets you control spinning direction of motors
// void directionControl() {
//   // Set motors to maximum speed
// //   digitalWrite(enA, HIGH);
// //   digitalWrite(enB, HIGH);

//   // Turn on motor A & B
// //   digitalWrite(in1, HIGH);
// //   digitalWrite(in2, LOW);
//   digitalWrite(in3, HIGH);
//   digitalWrite(in4, LOW);
//   delay(2000);

//   // Now change motor directions
// //   digitalWrite(in1, LOW);
// //   digitalWrite(in2, HIGH);
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, HIGH);
//   delay(2000);

//   // Turn off motors
// //   digitalWrite(in1, LOW);
// //   digitalWrite(in2, LOW);
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, LOW);
// }

// // This function lets you control speed of the motors
// void speedControl() {
//   // Turn on motors
// //   digitalWrite(in1, LOW);
// //   digitalWrite(in2, HIGH);
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, HIGH);

//   // Accelerate from zero to maximum speed
//   for (int i = 0; i < 256; i++) {
//     // analogWrite(enA, i);
//     analogWrite(enB, i);
//     delay(20);
//   }

//   // Decelerate from maximum speed to zero
//   for (int i = 255; i >= 0; --i) {
//     // analogWrite(enA, i);
//     analogWrite(enB, i);
//     delay(20);
//   }

//   // Now turn off motors
// //   digitalWrite(in1, LOW);
// //   digitalWrite(in2, LOW);
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, LOW);
// }

// void setup() {
//   // Set all the motor control pins to outputs
// //   pinMode(enA, OUTPUT);
//   pinMode(enB, OUTPUT);
// //   pinMode(in1, OUTPUT);
// //   pinMode(in2, OUTPUT);
//   pinMode(in3, OUTPUT);
//   pinMode(in4, OUTPUT);

//   // Turn off motors - Initial state
// //   digitalWrite(in1, LOW);
// //   digitalWrite(in2, LOW);
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, LOW);
// }

// void loop() {
//   directionControl();
//   delay(1000);
//   speedControl();
//   delay(1000);
// }