#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include "RoverConfig.h"
#include "RoverControls.h"

class Steering {
public:
    Steering() = default;

    void begin() {
        frontLeft.attach(FL_PIN);
        frontRight.attach(FR_PIN);
        rearLeft.attach(BL_PIN);
        rearRight.attach(BR_PIN);
        center();
    }

    void center() {
        setSteeringAngle(0.0f);
    }

    void setSteeringAngle(float steering) {
        float normalized = constrain(steering, -1.0f, 1.0f);
        int angle = 90 + (int)round(normalized * kMaxAngleDeg);

        frontLeft.write(angle);
        frontRight.write(angle);
        rearLeft.write(angle);
        rearRight.write(angle);
    }

    void applyControl(const RoverControlState& controls) {
        setSteeringAngle(controls.steering);
    }

    void servo_test() {
        for (int angle = 60; angle <= 120; angle += 10) {
            frontLeft.write(angle);
            frontRight.write(angle);
            rearLeft.write(angle);
            rearRight.write(angle);
            delay(150);
        }
        center();
    }

private:
    static constexpr int kMaxAngleDeg = 30;

    Servo frontLeft;
    Servo frontRight;
    Servo rearLeft;
    Servo rearRight;
}; 